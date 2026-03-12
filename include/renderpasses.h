#ifndef WW_RENDERPASSES_H
#define WW_RENDERPASSES_H

#if __has_include(<hyprland/src/desktop/Window.hpp>)
#include <hyprland/src/desktop/Window.hpp>
#elif __has_include(<hyprland/src/desktop/view/Window.hpp>)
#include <hyprland/src/desktop/view/Window.hpp>
#else
#error "Could not find Hyprland Window.hpp header"
#endif
#include <hyprland/src/render/Shader.hpp>
#include <hyprland/src/render/pass/PassElement.hpp>
#include <optional>

#if __has_include(<hyprland/src/event/EventBus.hpp>)
#define WW_HAS_EVENT_BUS 1
#else
#define WW_HAS_EVENT_BUS 0
#endif

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
    static inline constexpr unsigned int s_SUBDIVS = 18;
    static_assert(s_SUBDIVS > 0);

    static inline GLuint s_VAO, s_VBO, s_VBO_UVs, s_EBO;

  public:
    static inline std::vector<float> s_baseVerts;

    explicit CRenderWobblyWindowPassElement(
        CFramebuffer* pOldFramebuffer,
        PHLWINDOWREF pWindow
    ) :
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

    bool disableSimplification() override {
        return true;
    }

  private:
    CFramebuffer* m_pOldFramebuffer;
    PHLWINDOWREF m_pWindow;

#if WW_HAS_EVENT_BUS
    static inline SP<CShader> s_shader = nullptr;
#else
    static inline SShader* s_shader = nullptr;
#endif
};

#endif
