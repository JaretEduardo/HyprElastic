#ifndef WW_RENDERPASSES_H
#define WW_RENDERPASSES_H

#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/render/Shader.hpp>
#include <hyprland/src/render/pass/PassElement.hpp>
#include <optional>

class CFramebuffer;

class CBindOwnFramebufferPassElement final: public IPassElement {
  public:
    explicit CBindOwnFramebufferPassElement(CFramebuffer* pFramebuffer) :
        m_pFramebuffer {pFramebuffer} {}

    void draw(const CRegion& damage) override;

    bool needsLiveBlur() override {
        return false;
    }

    bool needsPrecomputeBlur() override {
        return false;
    }

    const char* passName() override {
        return "BIND_OWN_FRAMEBUFFER";
    }

    std::optional<CBox> boundingBox() override {
        return std::nullopt;
    }

  private:
    CFramebuffer* m_pFramebuffer;
};

class CRenderWobblyWindowPassElement final: public IPassElement {
    static inline constexpr unsigned int s_SUBDIVS = 8;
    static_assert(s_SUBDIVS > 0);

    static inline GLuint s_VAO, s_VBO, s_VBO_UVs, s_EBO;

  public:
    static inline std::vector<float> s_baseVerts;

    explicit CRenderWobblyWindowPassElement(CFramebuffer* pOldFramebuffer, PHLWINDOWREF pWindow) :
        m_pOldFramebuffer {pOldFramebuffer},
        m_pWindow {pWindow} {}

    static void initGPUObjects();
    static void deinitGPUObjects();

    void draw(const CRegion& damage) override;

    bool needsLiveBlur() override {
        return true;
    }

    bool needsPrecomputeBlur() override {
        return false;
    }

    const char* passName() override {
        return "RENDER_WOBBLY_WINDOW";
    }

    std::optional<CBox> boundingBox() override;

  private:
    CFramebuffer* m_pOldFramebuffer;
    PHLWINDOWREF m_pWindow;

    static inline SShader s_shader;
};

#endif
