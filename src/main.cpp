#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/event/EventBus.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/view/Window.hpp>
#include "SpringPhysics.hpp"
#include <fstream>
#include <any>

inline HANDLE PHANDLE = nullptr;
SpringPhysics g_physics(50.0f, 4.0f);
std::ofstream logFile;

Hyprutils::Signal::CHyprSignalListener g_TickListener;

void onTick() {
    PHLWINDOW pActiveWindow = nullptr;
    for (auto& w : g_pCompositor->m_windows) {
        if (g_pCompositor->isWindowActive(w)) {
            pActiveWindow = w;
            break;
        }
    }

    if (pActiveWindow) {
        float windowX = pActiveWindow->m_realPosition->value().x;
        g_physics.setTarget(windowX);
    }
    
    g_physics.update(0.016f);

    static int frameCount = 0;
    if (frameCount++ % 60 == 0) {
        logFile << "[Physics] Spring position: " << g_physics.getPosition() << "\n";
        logFile.flush();
    }
}

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;
    logFile.open("/tmp/hyprelastic.log", std::ios::out | std::ios::trunc);
    logFile << "--- Gelatin Successfully Connected ---\n";

    g_TickListener = Event::bus()->m_events.tick.registerListener([](std::any data) {
        onTick();
    });

    return {"HyprElastic", "Wobbly windows", "Jaret Eduardo", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    logFile.close();
}