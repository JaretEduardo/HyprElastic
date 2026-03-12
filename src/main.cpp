#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/event/EventBus.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/view/Window.hpp>
#include "WobblyWindow.hpp"
#include <vector>
#include <memory>
#include <algorithm>

inline HANDLE PHANDLE = nullptr;
std::vector<std::unique_ptr<CWobblyWindow>> g_vWobblyWindows;

Hyprutils::Signal::CHyprSignalListener g_TickListener;

void onTick() {
    for (auto& w : g_pCompositor->m_windows) {
        auto it = std::find_if(g_vWobblyWindows.begin(), g_vWobblyWindows.end(), [&](const auto& ww) {
            return ww->m_pWindow == w;
        });

        if (it == g_vWobblyWindows.end()) {
            g_vWobblyWindows.push_back(std::make_unique<CWobblyWindow>(w));
        }
    }

    std::erase_if(g_vWobblyWindows, [](const auto& ww) {
        return !ww->m_pWindow || std::find(g_pCompositor->m_windows.begin(), g_pCompositor->m_windows.end(), ww->m_pWindow) == g_pCompositor->m_windows.end();
    });

    for (auto& ww : g_vWobblyWindows) {
        ww->update();
    }
}

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;
    
    g_TickListener = Event::bus()->m_events.tick.registerListener([](std::any data) {
        onTick();
    });

    return {"HyprElastic", "Wobbly windows manager", "Jaret Eduardo", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    g_vWobblyWindows.clear();
}