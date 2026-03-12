#include "RenderWobbly.hpp"
#include "WobblyWindow.hpp"
#include <hyprland/src/Compositor.hpp>
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

    Vector2D offset = m_pPhysics->getOffset();

    offset.x = std::clamp(offset.x, -50.0, 50.0);
    offset.y = std::clamp(offset.y, -50.0, 50.0);
}

eDecorationType CRenderWobbly::getDecorationType() {
    return DECORATION_CUSTOM;
}

void CRenderWobbly::updateWindow(PHLWINDOW pWindow) {
    m_pWindow = pWindow;
}

void CRenderWobbly::damageEntire() {}