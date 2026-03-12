#include "WobblyWindow.hpp"
#include "RenderWobbly.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>

CWobblyWindow::CWobblyWindow(PHLWINDOW pWindow) 
    : m_pWindow(pWindow) 
{
    if (!pWindow) return;

    Vector2D pos = pWindow->m_realPosition->value();
    Vector2D size = pWindow->m_realSize->value();

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            m_grid[y][x].x = SpringPhysics(150.0f, 12.0f);
            m_grid[y][x].y = SpringPhysics(150.0f, 12.0f);

            float targetX = pos.x + (x * (size.x / (GRID_SIZE - 1)));
            float targetY = pos.y + (y * (size.y / (GRID_SIZE - 1)));

            m_grid[y][x].x.setTarget(targetX);
            m_grid[y][x].y.setTarget(targetY);
        }
    }

    HyprlandAPI::addWindowDecoration(PHANDLE, pWindow, makeUnique<CRenderWobbly>(pWindow, this));
}

void CWobblyWindow::update() {
    if (!m_pWindow) return;

    Vector2D pos = m_pWindow->m_realPosition->value();
    Vector2D size = m_pWindow->m_realSize->value();

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            float targetX = pos.x + (x * (size.x / (GRID_SIZE - 1)));
            float targetY = pos.y + (y * (size.y / (GRID_SIZE - 1)));

            m_grid[y][x].x.setTarget(targetX);
            m_grid[y][x].y.setTarget(targetY);

            m_grid[y][x].x.update(0.016f);
            m_grid[y][x].y.update(0.016f);
        }
    }
}

Vector2D CWobblyWindow::getOffsetAt(float relX, float relY) {
    int gx = std::clamp((int)(relX * (GRID_SIZE - 1)), 0, GRID_SIZE - 1);
    int gy = std::clamp((int)(relY * (GRID_SIZE - 1)), 0, GRID_SIZE - 1);

    float offX = m_grid[gy][gx].x.getPosition() - (m_pWindow->m_realPosition->value().x + (gx * (m_pWindow->m_realSize->value().x / (GRID_SIZE - 1))));
    float offY = m_grid[gy][gx].y.getPosition() - (m_pWindow->m_realPosition->value().y + (gy * (m_pWindow->m_realSize->value().y / (GRID_SIZE - 1))));

    return {offX, offY};
}