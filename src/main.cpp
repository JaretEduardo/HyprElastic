#include <hyprland/src/plugins/PluginAPI.hpp>
#include "SpringPhysics.hpp"
#include <string>
#include <fstream> // La librería estándar para leer/escribir archivos en C++

inline HANDLE PHANDLE = nullptr;
SpringPhysics g_physics(50.0f, 4.0f);

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    std::ofstream logFile("/tmp/hyprelastic.log", std::ios::app);
    
    logFile << "----------------------------------------\n";
    logFile << "[HyprElastic] 🟢 Engine started. Initializing test simulation...\n";

    g_physics.setTarget(100.0f);
    
    for(int i = 0; i < 5; i++) {
        g_physics.update(0.016f);
        logFile << "[HyprElastic] Simulating... Current Position: " << g_physics.getPosition() << "\n";
    }

    logFile.close();

    return {"HyprElastic", "Wobbly windows physics engine", "Jaret Eduardo", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
}