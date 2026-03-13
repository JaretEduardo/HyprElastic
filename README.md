# HyprElastic

![Lines of Code](./doc/loc-badge.svg)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Hyprland](https://img.shields.io/badge/Hyprland-33CCFF?style=flat)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![Meson](https://img.shields.io/badge/Meson-8B1D2E?style=flat)

Hyprland plugin that adds a "wobbly windows" effect using a deformable mesh and a spring-damper model in C++.

## How it works

HyprElastic hooks into window rendering (`renderWindow`) and applies this flow:

1. Captures the window into a framebuffer.
2. Deforms a subdivided UV mesh (grid) in real time.
3. Renders the deformed result back onto the scene.

The core physics model is:

$$F = -k \cdot x - c \cdot v$$

- $k$: spring stiffness.
- $c$: damping.
- $x$: displacement.
- $v$: velocity.

The plugin includes clamping and hysteresis to reduce flicker, mesh inversion, and visual jumps.

## Compatibility

- Hyprland 0.54.x (host) with additional compatibility for 0.50.x APIs.
- Linux.
- Built with Meson + Ninja.

Important: the `.so` must be compiled against the same ABI/headers as the Hyprland instance where it will be loaded. If you build in one environment (for example `nix develop`) and load on a different host, it may fail due to symbol/ABI mismatches.

## Installation

### Option 1: Manual build

```bash
git clone https://github.com/JaretEduardo/HyprElastic.git
cd HyprElastic
/usr/bin/meson setup build-native
/usr/bin/meson compile -C build-native
```

### Option 2: Nix

You can use `flake.nix`/`plugin.nix`, but make sure to load the plugin in a Hyprland session compatible with that build.

## Quick usage

The project includes `run.sh` with 3 modes:

```bash
chmod +x run.sh

# Build and hot-reload in your current Hyprland session
./run.sh dev

# Build only
./run.sh build

# Launch a test session with hyprlandd.conf
./run.sh nested
```

Optional variables:

```bash
BUILD_DIR=build-native MESON_BIN=/usr/bin/meson ./run.sh dev
```

## Load plugin manually

If you are not using `./run.sh dev`, you can load it manually:

```bash
hyprctl plugin load /absolute/path/libwigglewobble.so
hyprctl plugin list
```

To reload cleanly, unload the previous instance first:

```bash
hyprctl plugin unload /absolute/path/libwigglewobble.so
```

## Sharing with your team

Recommended workflow so it works for everyone:

1. Clone the repository.
2. Build locally on each machine/Hyprland session.
3. Use `./run.sh dev` for safe hot-reload.

This avoids linker cache issues and ABI incompatibility across environments.

## Troubleshooting

- Clipping or flicker on an empty workspace:
    - Update to the latest commit (includes damage/scissor and monitor-sized framebuffer fixes).
- `undefined symbol` on load:
    - Rebuild in the same environment where Hyprland is running (do not mix Nix build with a different host runtime).
- Plugin loads twice:
    - Use `./run.sh dev` (it unloads old instances and loads a unique timestamped copy).

## Contributions

Issues and PRs are welcome.

Author: Jaret Eduardo