#pragma once
#include <hyprland/src/plugins/PluginAPI.hpp>
#include "SpringPhysics.hpp"
#include <hyprland/src/desktop/view/Window.hpp>
#include <vector>

extern HANDLE PHANDLE;

const int GRID_SIZE = 4; 

struct SGridPoint {
    SpringPhysics x;
    SpringPhysics y;
};

class CWobblyWindow {
public:
    CWobblyWindow(PHLWINDOW pWindow);
    void update();
    
    Vector2D getOffsetAt(float relX, float relY);

    PHLWINDOW m_pWindow;

private:
    SGridPoint m_grid[GRID_SIZE][GRID_SIZE];
};