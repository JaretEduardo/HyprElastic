#include "RenderWobbly.hpp"
#include "WobblyWindow.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/render/OpenGL.hpp>
#include <hyprland/src/render/decorations/DecorationPositioner.hpp>

CRenderWobbly::CRenderWobbly(PHLWINDOW pWindow, CWobblyWindow* pPhysics) 
    : IHyprWindowDecoration(pWindow), m_pWindow(pWindow), m_pPhysics(pPhysics) {
}

SDecorationPositioningInfo CRenderWobbly::getPositioningInfo() {
    SDecorationPositioningInfo info;
    info.policy = DECORATION_POSITION_ABSOLUTE; 
    return info;
}

void CRenderWobbly::onPositioningReply(const SDecorationPositioningReply& reply) {}

void CRenderWobbly::draw(PHLMONITOR pMonitor, float const& a) {
    if (!m_pWindow || !m_pPhysics) return;

    Vector2D offset = m_pPhysics->getOffsetAt(0.5f, 0.5f);

    offset.x = std::clamp(offset.x, -100.0, 100.0);
    offset.y = std::clamp(offset.y, -100.0, 100.0);

    CBox shadowBox = {
        m_pWindow->m_realPosition->value().x + offset.x,
        m_pWindow->m_realPosition->value().y + offset.y,
        m_pWindow->m_realSize->value().x,
        m_pWindow->m_realSize->value().y
    };

    CHyprColor dynamicColor = m_pWindow->m_realBorderColor.m_colors[0];
    dynamicColor.a = 0.4f * a;

    CHyprOpenGLImpl::SRectRenderData renderData;
    renderData.round = (int)m_pWindow->rounding();

    g_pHyprOpenGL->renderRect(shadowBox, dynamicColor, renderData);
}

eDecorationType CRenderWobbly::getDecorationType() {
    return DECORATION_CUSTOM;
}

void CRenderWobbly::updateWindow(PHLWINDOW pWindow) {
    m_pWindow = pWindow;
}

void CRenderWobbly::damageEntire() {}