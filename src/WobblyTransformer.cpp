#include "WobblyTransformer.hpp"
#include "WobblyWindow.hpp"
#include <hyprland/src/render/OpenGL.hpp>
#include <hyprland/src/desktop/view/Window.hpp>

CWobblyTransformer::CWobblyTransformer(CWobblyWindow* pPhysics) : m_pPhysics(pPhysics) {
}

CFramebuffer* CWobblyTransformer::transform(CFramebuffer* in) {
    if (!in || !m_pPhysics || !m_pPhysics->m_pWindow)
        return in;

    auto pWindow = m_pPhysics->m_pWindow;
    auto pTexture = in->getTexture();

    for (int y = 0; y < GRID_SIZE - 1; y++) {
        for (int x = 0; x < GRID_SIZE - 1; x++) {
            
            float x1 = (float)x / (GRID_SIZE - 1);
            float y1 = (float)y / (GRID_SIZE - 1);
            float x2 = (float)(x + 1) / (GRID_SIZE - 1);
            float y2 = (float)(y + 1) / (GRID_SIZE - 1);

            Vector2D offTL = m_pPhysics->getOffsetAt(x1, y1);
            Vector2D offBR = m_pPhysics->getOffsetAt(x2, y2);

            CBox tileBox = {
                pWindow->m_realPosition->value().x + (x1 * pWindow->m_realSize->value().x) + offTL.x,
                pWindow->m_realPosition->value().y + (y1 * pWindow->m_realSize->value().y) + offTL.y,
                ((x2 - x1) * pWindow->m_realSize->value().x) + (offBR.x - offTL.x),
                ((y2 - y1) * pWindow->m_realSize->value().y) + (offBR.y - offTL.y)
            };

            CHyprOpenGLImpl::STextureRenderData renderData;
            renderData.allowCustomUV = true;
            renderData.a = 1.0f; 

            g_pHyprOpenGL->m_renderData.primarySurfaceUVTopLeft     = Vector2D(x1, y1);
            g_pHyprOpenGL->m_renderData.primarySurfaceUVBottomRight  = Vector2D(x2, y2);

            g_pHyprOpenGL->renderTexture(pTexture, tileBox, renderData);
        }
    }

    g_pHyprOpenGL->m_renderData.primarySurfaceUVTopLeft     = Vector2D(-1, -1);
    g_pHyprOpenGL->m_renderData.primarySurfaceUVBottomRight  = Vector2D(-1, -1);

    return in; 
}

void CWobblyTransformer::preWindowRender(CSurfacePassElement::SRenderData* pRenderData) {
    pRenderData->alpha = 0.0f; 
}

void IWindowTransformer::preWindowRender(CSurfacePassElement::SRenderData* pRenderData) {
}