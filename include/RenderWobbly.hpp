#pragma once
#include <hyprland/src/render/decorations/IHyprWindowDecoration.hpp>
#include <hyprland/src/desktop/view/Window.hpp>

class CWobblyWindow; 

class CRenderWobbly : public IHyprWindowDecoration {
public:
    CRenderWobbly(PHLWINDOW pWindow, CWobblyWindow* pPhysics);
    virtual ~CRenderWobbly() = default;

    virtual SDecorationPositioningInfo getPositioningInfo() override;
    virtual void onPositioningReply(const SDecorationPositioningReply& reply) override;
    virtual void draw(PHLMONITOR pMonitor, float const& a) override;
    virtual eDecorationType getDecorationType() override;
    virtual void updateWindow(PHLWINDOW pWindow) override;
    virtual void damageEntire() override;

    virtual std::string getDisplayName() override { return "InokaiiWobbly"; }

private:
    PHLWINDOW      m_pWindow;
    CWobblyWindow* m_pPhysics;
};