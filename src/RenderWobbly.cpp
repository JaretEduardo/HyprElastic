#include "RenderWobbly.hpp"
#include "WobblyWindow.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/render/OpenGL.hpp>
#include <hyprland/src/render/Renderer.hpp> // Vital para decirle a la GPU que dibuje

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

    // Obtenemos qué tanto se ha estirado el centro de nuestra malla elástica
    Vector2D offset = m_pPhysics->getOffsetAt(0.5f, 0.5f);

    // HACEMOS LA CAJA MÁS GRANDE QUE LA VENTANA (20 píxeles extra por lado)
    // Así sobresale por los bordes y la ventana original no la puede tapar.
    CBox shadowBox = {
        m_pWindow->m_realPosition->value().x + offset.x - 20,
        m_pWindow->m_realPosition->value().y + offset.y - 20,
        m_pWindow->m_realSize->value().x + 40,
        m_pWindow->m_realSize->value().y + 40
    };

    // UN ROJO CHILLÓN AL 100% DE OPACIDAD (Imposible no verlo)
    CHyprColor debugColor = {1.0f, 0.0f, 0.0f, 1.0f}; 

    CHyprOpenGLImpl::SRectRenderData renderData;
    renderData.round = (int)m_pWindow->rounding();

    // Dibujamos el rectángulo de gelatina roja
    g_pHyprOpenGL->renderRect(shadowBox, debugColor, renderData);
}

eDecorationType CRenderWobbly::getDecorationType() {
    return DECORATION_CUSTOM;
}

void CRenderWobbly::updateWindow(PHLWINDOW pWindow) {
    m_pWindow = pWindow;
}

// ESTA ERA LA PIEZA FALTANTE: Avisar a la GPU que el área se está moviendo
void CRenderWobbly::damageEntire() {
    if (!m_pWindow) return;
    
    // Le decimos a Hyprland que redibuje un área enorme alrededor de la ventana
    // para asegurarnos de que no nos corte la animación.
    CBox damageBox = {
        m_pWindow->m_realPosition->value().x - 100,
        m_pWindow->m_realPosition->value().y - 100,
        m_pWindow->m_realSize->value().x + 200,
        m_pWindow->m_realSize->value().y + 200
    };
    g_pHyprRenderer->damageBox(damageBox);
}