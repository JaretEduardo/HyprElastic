#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/event/EventBus.hpp>
#include "SpringPhysics.hpp"
#include <fstream>
#include <any>

inline HANDLE PHANDLE = nullptr;
SpringPhysics g_physics(50.0f, 4.0f);
std::ofstream logFile;

Hyprutils::Signal::CHyprSignalListener g_TickListener;

void onTick() {
    g_physics.update(0.016f);
    logFile << "[Tick] Position: " << g_physics.getPosition() << "\n";
    logFile.flush();
}

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;
    
    logFile.open("/tmp/hyprelastic.log", std::ios::out | std::ios::trunc);
    logFile << "--- Gelatina Conectada al EventBus ---\n";
    g_physics.setTarget(100.0f);

    g_TickListener = Event::bus()->m_events.tick.registerListener([](std::any data) {
        onTick();
    });

    return {"HyprElastic", "Wobbly windows physics engine", "Jaret Eduardo", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    logFile.close();
}