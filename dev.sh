#!/bin/bash

echo "🔄 Starting HyprElastic Hot-Reload..."

LAST_PLUGIN=$(cat .last_plugin 2>/dev/null)
if [ -n "$LAST_PLUGIN" ]; then
    hyprctl plugin unload "$LAST_PLUGIN" >/dev/null 2>&1
    rm "$LAST_PLUGIN" 2>/dev/null
fi

ninja -C build
if [ $? -ne 0 ]; then
    echo "❌ Compilation error. Please check your C++ code."
    exit 1
fi

HASH=$(date +%s)
NEW_PLUGIN="$(pwd)/build/libhyprelastic_${HASH}.so"
cp build/libhyprelastic.so "$NEW_PLUGIN"

hyprctl plugin load "$NEW_PLUGIN"
echo "$NEW_PLUGIN" > .last_plugin

echo "✅ Jelly successfully injected into memory."