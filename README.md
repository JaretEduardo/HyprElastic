# HyprElastic

![Lines of Code](./doc/loc-badge.svg)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Hyprland](https://img.shields.io/badge/Hyprland-33CCFF?style=flat)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![Meson](https://img.shields.io/badge/Meson-8B1D2E?style=flat)

HyprElastic is a high-performance plugin for Hyprland that implements a "wobbly windows" effect using a spring-mass physics engine written in C++. 

This project aims to provide a fluid and dynamic visual experience while maintaining the stability and low resource consumption required for a professional Linux environment.

---

## Features
- **Hookean Physics Engine:** Deformations calculated based on the interaction of masses and springs.
- **Zero Latency:** Optimized to integrate directly into the Hyprland rendering cycle.
- **Compatibility:** Built specifically for Hyprland v0.54.x and later.
- **Dynamic Configuration:** Adjust elasticity and friction parameters without restarting the compositor.

---

## Physics Engine
HyprElastic utilizes a numerical integration system to simulate the restoring force of a spring:

$$F = -k \cdot x - c \cdot v$$

Where:
- $k$ represents the **stiffness**.
- $c$ represents the **damping** (friction).
- $x$ and $v$ are the displacement and velocity of the vertices, respectively.

---

## Installation

### Prerequisites
- `hyprland-headers`
- `meson`
- `ninja`

### Using hyprpm (Recommended)
```bash
hyprpm add [https://github.com/YourUsername/HyprElastic](https://github.com/YourUsername/HyprElastic)
hyprpm enable HyprElastic
```

### Manual Build
```bash
git clone [https://github.com/YourUsername/HyprElastic](https://github.com/YourUsername/HyprElastic)
cd HyprElastic
meson setup build
ninja -C build
```

## Configuration
Add the following block to your hyprland.conf to customize the effect:

```Ini, TOML
plugin {
    hyprelastic {
        enabled = true
        
        # Physics Parameters
        stiffness = 50.0
        friction = 4.0
        sensitivity = 1.0
        
        # Behavior
        wobbly_maximize = true
        wobbly_fullscreen = false
    }
}
```

## Contributions

This is a personal project open to the community. If you find a bug or have an idea to improve the physics engine, please open an Issue or a Pull Request.

Author: Jaret Eduardo
Software Developer