#include <hyprland/src/plugins/PluginAPI.hpp>

inline HANDLE PHANDLE = nullptr;

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    return {"HyprElastic", "Wobbly windows physics engine", "Jaret Eduardo", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
}