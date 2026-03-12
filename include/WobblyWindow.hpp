#pragma once
#include "SpringPhysics.hpp"
#include <hyprland/src/desktop/view/Window.hpp>

class CWobblyWindow {
public:
    CWobblyWindow(PHLWINDOW pWindow);

    void update();

    Vector2D getOffset();

    PHLWINDOW m_pWindow;

private:
    SpringPhysics m_springX;
    SpringPhysics m_springY;
};