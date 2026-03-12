#include "WobblyWindow.hpp"
#include "RenderWobbly.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>

using namespace Hyprutils::Memory;

CWobblyWindow::CWobblyWindow(PHLWINDOW pWindow) 
    : m_pWindow(pWindow), 
      m_springX(160.0f, 10.0f),
      m_springY(160.0f, 10.0f) 
{
    if (!pWindow) return;

    float initX = pWindow->m_realPosition->value().x;
    float initY = pWindow->m_realPosition->value().y;
    m_springX.setTarget(initX);
    m_springY.setTarget(initY);

    HyprlandAPI::addWindowDecoration(PHANDLE, pWindow, makeUnique<CRenderWobbly>(pWindow, this));
}

void CWobblyWindow::update() {
    if (!m_pWindow) return;
    m_springX.setTarget(m_pWindow->m_realPosition->value().x);
    m_springY.setTarget(m_pWindow->m_realPosition->value().y);
    m_springX.update(0.016f);
    m_springY.update(0.016f);
}

Vector2D CWobblyWindow::getOffset() {
    if (!m_pWindow) return {0, 0};
    float offX = m_springX.getPosition() - m_pWindow->m_realPosition->value().x;
    float offY = m_springY.getPosition() - m_pWindow->m_realPosition->value().y;
    return {offX, offY};
}