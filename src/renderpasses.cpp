#include "renderpasses.h"

#include "globals.h"
#include "wobblywindow.h"

#include <GLES3/gl32.h>
#include <hyprland/src/render/Framebuffer.hpp>
#include <hyprland/src/render/OpenGL.hpp>
#include <hyprland/src/render/Renderer.hpp>

void CBindOwnFramebufferPassElement::draw(const CRegion& damage) {
    g_pHyprOpenGL->scissor(nullptr);

    m_pFramebuffer->bind();
    g_pHyprOpenGL->m_renderData.currentFB = m_pFramebuffer;

    GLCALL(glClearColor(0, 0, 0, 0));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void CRenderWobblyWindowPassElement::initGPUObjects() {
    constexpr const char* FRAGSRC = R"glsl(#version 300 es

precision highp float;
in vec2 v_texcoord; // is in 0-1
uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;
void main() {
    fragColor = texture(tex, v_texcoord);
}
)glsl";

    s_shader.program = g_pHyprOpenGL->createProgram(g_pHyprOpenGL->m_shaders->TEXVERTSRC, FRAGSRC);
    s_shader.uniformLocations[SHADER_PROJ] = glGetUniformLocation(s_shader.program, "proj");
    s_shader.uniformLocations[SHADER_TEX] = glGetUniformLocation(s_shader.program, "tex");
    s_shader.uniformLocations[SHADER_TEX_ATTRIB] =
        glGetAttribLocation(s_shader.program, "texcoord");
    s_shader.uniformLocations[SHADER_POS_ATTRIB] = glGetAttribLocation(s_shader.program, "pos");

    const unsigned int vertsPerRow = s_SUBDIVS + 1;
    s_baseVerts.reserve(vertsPerRow * vertsPerRow * 2);

    const float step = 1.f / (s_SUBDIVS);
    for (unsigned int y = 0; y < vertsPerRow; ++y) {
        for (unsigned int x = 0; x < vertsPerRow; ++x) {
            s_baseVerts.push_back(x * step);
            s_baseVerts.push_back(y * step);
        }
    }

    std::vector<GLuint> indices;
    indices.reserve(3 * 2 * s_SUBDIVS * s_SUBDIVS);

    for (unsigned int y = 0; y < s_SUBDIVS; ++y) {
        for (unsigned int x = 0; x < s_SUBDIVS; ++x) {
            indices.push_back(y * vertsPerRow + x + 1); // top right
            indices.push_back(y * vertsPerRow + x); // top left
            indices.push_back((y + 1) * vertsPerRow + x + 1); // bottom right

            indices.push_back(y * vertsPerRow + x); // top left
            indices.push_back((y + 1) * vertsPerRow + x); // bottom left
            indices.push_back((y + 1) * vertsPerRow + x + 1); // bottom right
        }
    }

    GLCALL(glGenVertexArrays(1, &s_VAO));
    GLCALL(glGenBuffers(1, &s_VBO));
    GLCALL(glGenBuffers(1, &s_VBO_UVs));
    GLCALL(glGenBuffers(1, &s_EBO));

    GLCALL(glBindVertexArray(s_VAO));

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO));
    GLCALL(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(GLuint),
        indices.data(),
        GL_STATIC_DRAW
    ));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_VBO));
    {
        GLCALL(glBufferData(
            GL_ARRAY_BUFFER,
            s_baseVerts.size() * sizeof(float),
            s_baseVerts.data(),
            GL_DYNAMIC_DRAW
        ));

        GLCALL(glEnableVertexAttribArray(s_shader.uniformLocations[SHADER_POS_ATTRIB]));
        GLCALL(glVertexAttribPointer(
            s_shader.uniformLocations[SHADER_POS_ATTRIB],
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
        ));
    }

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_VBO_UVs));
    {
        GLCALL(glBufferData(
            GL_ARRAY_BUFFER,
            s_baseVerts.size() * sizeof(float),
            s_baseVerts.data(),
            GL_DYNAMIC_DRAW
        )); // Initial dummy UVs
        GLCALL(glEnableVertexAttribArray(s_shader.uniformLocations[SHADER_TEX_ATTRIB]));
        GLCALL(glVertexAttribPointer(
            s_shader.uniformLocations[SHADER_TEX_ATTRIB],
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
        ));
    }

    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void CRenderWobblyWindowPassElement::deinitGPUObjects() {
    GLCALL(glDeleteVertexArrays(1, &s_VAO));
    GLCALL(glDeleteBuffers(1, &s_VBO));
    GLCALL(glDeleteBuffers(1, &s_VBO_UVs));
    GLCALL(glDeleteBuffers(1, &s_EBO));
}

void CRenderWobblyWindowPassElement::draw(const CRegion& damage) {
    g_pHyprOpenGL->scissor(nullptr);
    auto* const pWindowFB = g_pHyprOpenGL->m_renderData.currentFB;

    m_pOldFramebuffer->bind();
    g_pHyprOpenGL->m_renderData.currentFB = m_pOldFramebuffer;

    const auto windowBox = m_pWindow->getFullWindowBoundingBox();

    CBox newBox = windowBox;
    g_pHyprOpenGL->m_renderData.renderModif.applyToBox(newBox);

    // get transform
    const auto TRANSFORM =
        wlTransformToHyprutils(invertTransform(g_pHyprOpenGL->m_renderData.pMonitor->m_transform));

    Mat3x3 matrix =
        g_pHyprOpenGL->m_renderData.monitorProjection.projectBox(newBox, TRANSFORM, newBox.rot);
    Mat3x3 glMatrix = g_pHyprOpenGL->m_renderData.projection.copy().multiply(matrix);

    GLCALL(glActiveTexture(GL_TEXTURE0));
    pWindowFB->getTexture()->bind();

    g_pHyprOpenGL->useProgram(s_shader.program);
    s_shader.setUniformMatrix3fv(SHADER_PROJ, 1, GL_TRUE, glMatrix.getMatrix());
    s_shader.setUniformInt(SHADER_TEX, 0);

    GLCALL(glBindVertexArray(s_VAO));

    const Vector2D UVSize = Vector2D {windowBox.width, windowBox.height} / pWindowFB->m_size;

    const unsigned int vertsPerRow = s_SUBDIVS + 1;
    std::vector<float> UVs;
    UVs.reserve(vertsPerRow * vertsPerRow * 2);

    const auto step = UVSize / s_SUBDIVS;

    for (unsigned int y = 0; y < vertsPerRow; ++y) {
        const float v = y * step.y;
        for (unsigned int x = 0; x < vertsPerRow; ++x) {
            const float u = x * step.x;

            UVs.push_back(u);
            UVs.push_back(v);
        }
    }

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_VBO_UVs));
    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, UVs.size() * sizeof(float), UVs.data()));

    if (g_wobblyWindows.contains(m_pWindow)) {
        auto&& wobble = g_wobblyWindows[m_pWindow];

        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_VBO));
        GLCALL(glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            wobble.m_particlePositions.size() * sizeof(float),
            wobble.m_particlePositions.data()
        ));
    } else {
        // restore default verts
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_VBO));
        GLCALL(glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            s_baseVerts.size() * sizeof(float),
            s_baseVerts.data()
        ));
    }

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO));
    GLCALL(glDrawElements(GL_TRIANGLE_STRIP, 3 * 2 * s_SUBDIVS * s_SUBDIVS, GL_UNSIGNED_INT, 0));

    GLCALL(glBindVertexArray(0));
    pWindowFB->getTexture()->unbind();
}

std::optional<CBox> CRenderWobblyWindowPassElement::boundingBox() {
    auto ret = m_pWindow->getFullWindowBoundingBox();
    const auto wobbleBox = g_wobblyWindows.at(PHLWINDOWREF {m_pWindow}).getBox();
    ret.scale(Vector2D {wobbleBox.width, wobbleBox.height});
    ret.translate(Vector2D {wobbleBox.x, wobbleBox.y});

    return ret;
}
