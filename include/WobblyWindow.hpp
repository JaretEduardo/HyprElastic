#pragma once
#include <hyprland/src/desktop/view/Window.hpp>
#include <hyprutils/math/Vector2D.hpp>

using namespace Hyprutils::Math;

#define GRID_SIZE 4

struct SpringNode {
    float posX = 0, posY = 0;
    float velX = 0, velY = 0;
    float targetX = 0, targetY = 0;
    float forceX = 0, forceY = 0;
};

class CWobblyWindow {
public:
    PHLWINDOW m_pWindow;
    SpringNode m_grid[GRID_SIZE][GRID_SIZE];

    CWobblyWindow(PHLWINDOW pWindow);
    
    void update(float deltaTime);
    Vector2D getOffsetAt(float u, float v);
};