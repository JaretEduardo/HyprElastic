#pragma once
#include <hyprland/src/plugins/PluginAPI.hpp>
#include "SpringPhysics.hpp"
#include <hyprland/src/desktop/view/Window.hpp>

extern HANDLE PHANDLE;

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