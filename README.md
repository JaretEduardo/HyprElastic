# HyprElastic

![Lines of Code](./doc/loc-badge.svg)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Hyprland](https://img.shields.io/badge/Hyprland-33CCFF?style=flat)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![Meson](https://img.shields.io/badge/Meson-8B1D2E?style=flat)

Plugin de Hyprland que agrega efecto "wobbly windows" (ventanas elásticas) usando una malla deformable y un modelo de resorte-amortiguador en C++.

## Como funciona

HyprElastic se engancha al render de ventanas (`renderWindow`) y aplica este flujo:

1. Captura la ventana en un framebuffer.
2. Deforma una malla UV subdividida (grid) en tiempo real.
3. Renderiza el resultado deformado sobre la escena.

La fisica base sigue:

$$F = -k \cdot x - c \cdot v$$

- $k$: rigidez del resorte.
- $c$: amortiguacion.
- $x$: desplazamiento.
- $v$: velocidad.

El plugin incluye clamps y histeresis para evitar flicker, inversion de malla y saltos visuales.

## Compatibilidad

- Hyprland 0.54.x (host) y compatibilidad adicional con APIs de 0.50.x.
- Linux.
- Compilacion con Meson + Ninja.

Importante: el `.so` debe compilarse contra la misma ABI/headers de la instancia de Hyprland donde se va a cargar. Si compilas en un entorno (por ejemplo `nix develop`) y cargas en otro host distinto, puede fallar por simbolos/ABI.

## Instalacion

### Opcion 1: Compilar manual

```bash
git clone https://github.com/JaretEduardo/HyprElastic.git
cd HyprElastic
/usr/bin/meson setup build-native
/usr/bin/meson compile -C build-native
```

### Opcion 2: Nix

Puedes usar `flake.nix`/`plugin.nix`, pero asegurate de cargar el plugin en una sesion Hyprland compatible con ese build.

## Uso rapido

El proyecto incluye `run.sh` con 3 modos:

```bash
chmod +x run.sh

# Compila y hot-reload en tu sesion actual de Hyprland
./run.sh dev

# Solo compila
./run.sh build

# Abre una sesion de prueba con hyprlandd.conf
./run.sh nested
```

Variables opcionales:

```bash
BUILD_DIR=build-native MESON_BIN=/usr/bin/meson ./run.sh dev
```

## Cargar plugin manualmente

Si no usas `./run.sh dev`, puedes cargar manual:

```bash
hyprctl plugin load /ruta/absoluta/libwigglewobble.so
hyprctl plugin list
```

Para actualizar sin residuos, primero descarga la instancia anterior:

```bash
hyprctl plugin unload /ruta/absoluta/libwigglewobble.so
```

## Compartir en tu grupo

Recomendacion para que les funcione a todos:

1. Clonar repo.
2. Compilar localmente en su propia maquina/sesion de Hyprland.
3. Usar `./run.sh dev` para hot-reload seguro.

Esto evita problemas de cache del linker y de incompatibilidad ABI entre entornos.

## Troubleshooting

- Se corta/parpadea en workspace vacio:
    - Actualiza al ultimo commit (incluye fixes de damage/scissor y framebuffer monitor-sized).
- `undefined symbol` al cargar:
    - Recompila en el mismo entorno donde corre Hyprland (no mezclar build de Nix con host distinto).
- Se carga dos veces:
    - Usa `./run.sh dev` (desmonta instancias viejas y carga una copia unica por timestamp).

## Contribuciones

Issues y PRs son bienvenidos.

Autor: Jaret Eduardo