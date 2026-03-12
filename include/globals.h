#ifndef WW_GLOBALS_H
#define WW_GLOBALS_H

#include <hyprland/src/desktop/DesktopTypes.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/render/Framebuffer.hpp>
#include <map>

inline HANDLE PHANDLE = nullptr;

class CWobblyWindow;

extern std::map<PHLWINDOWREF, CFramebuffer> g_windowFramebuffers;
extern std::map<PHLWINDOWREF, Vector2D> g_windowPositions;
extern std::map<PHLWINDOWREF, CWobblyWindow> g_wobblyWindows;

#endif
