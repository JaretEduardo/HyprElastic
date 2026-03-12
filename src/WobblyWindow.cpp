#include "WobblyWindow.hpp"
#include <algorithm>

CWobblyWindow::CWobblyWindow(PHLWINDOW pWindow) : m_pWindow(pWindow) {
    if (!m_pWindow) return;

    auto pos = pWindow->m_realPosition->value();
    auto size = pWindow->m_realSize->value();

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            float targetX = pos.x + (x * (size.x / (GRID_SIZE - 1)));
            float targetY = pos.y + (y * (size.y / (GRID_SIZE - 1)));

            m_grid[y][x].posX = targetX;
            m_grid[y][x].posY = targetY;
            m_grid[y][x].targetX = targetX;
            m_grid[y][x].targetY = targetY;
        }
    }
}

void CWobblyWindow::update(float deltaTime) {
    if (!m_pWindow) return;

    auto pos = m_pWindow->m_realPosition->value();
    auto size = m_pWindow->m_realSize->value();

    float k_target = 10.0f;
    float k_neighbor = 20.0f;
    float friction = 3.5f;

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            auto& node = m_grid[y][x];

            node.targetX = pos.x + (x * (size.x / (GRID_SIZE - 1)));
            node.targetY = pos.y + (y * (size.y / (GRID_SIZE - 1)));

            float fX = k_target * (node.targetX - node.posX);
            float fY = k_target * (node.targetY - node.posY);

            if (x > 0) {
                fX += k_neighbor * (m_grid[y][x-1].posX - node.posX);
                fY += k_neighbor * (m_grid[y][x-1].posY - node.posY);
            }
            if (x < GRID_SIZE - 1) {
                fX += k_neighbor * (m_grid[y][x+1].posX - node.posX);
                fY += k_neighbor * (m_grid[y][x+1].posY - node.posY);
            }
            if (y > 0) {
                fX += k_neighbor * (m_grid[y-1][x].posX - node.posX);
                fY += k_neighbor * (m_grid[y-1][x].posY - node.posY);
            }
            if (y < GRID_SIZE - 1) {
                fX += k_neighbor * (m_grid[y+1][x].posX - node.posX);
                fY += k_neighbor * (m_grid[y+1][x].posY - node.posY);
            }

            fX -= friction * node.velX;
            fY -= friction * node.velY;

            node.forceX = fX;
            node.forceY = fY;
        }
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            auto& node = m_grid[y][x];
            node.velX += node.forceX * deltaTime;
            node.velY += node.forceY * deltaTime;
            node.posX += node.velX * deltaTime;
            node.posY += node.velY * deltaTime;
        }
    }
}

Vector2D CWobblyWindow::getOffsetAt(float u, float v) {
    int x = std::clamp((int)(u * (GRID_SIZE - 1)), 0, GRID_SIZE - 1);
    int y = std::clamp((int)(v * (GRID_SIZE - 1)), 0, GRID_SIZE - 1);

    return Vector2D(
        m_grid[y][x].posX - m_grid[y][x].targetX,
        m_grid[y][x].posY - m_grid[y][x].targetY
    );
}