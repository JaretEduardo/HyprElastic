#include "RenderWobbly.hpp"
#include "WobblyWindow.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/render/OpenGL.hpp>
#include <hyprland/src/render/Renderer.hpp>

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

    CBox shadowBox = {
        m_pWindow->m_realPosition->value().x + offset.x - 20,
        m_pWindow->m_realPosition->value().y + offset.y - 20,
        m_pWindow->m_realSize->value().x + 40,
        m_pWindow->m_realSize->value().y + 40
    };

    CHyprColor debugColor = {1.0f, 0.0f, 0.0f, 1.0f}; 

    CHyprOpenGLImpl::SRectRenderData renderData;
    renderData.round = (int)m_pWindow->rounding();

    g_pHyprOpenGL->renderRect(shadowBox, debugColor, renderData);
}

eDecorationType CRenderWobbly::getDecorationType() {
    return DECORATION_CUSTOM;
}

void CRenderWobbly::updateWindow(PHLWINDOW pWindow) {
    m_pWindow = pWindow;
}

void CRenderWobbly::damageEntire() {
    if (!m_pWindow) return;
    
    CBox damageBox = {
        m_pWindow->m_realPosition->value().x - 100,
        m_pWindow->m_realPosition->value().y - 100,
        m_pWindow->m_realSize->value().x + 200,
        m_pWindow->m_realSize->value().y + 200
    };
    g_pHyprRenderer->damageBox(damageBox);
}