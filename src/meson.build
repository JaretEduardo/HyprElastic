shared_module('wigglewobble',
  ['main.cpp','wobblywindow.cpp','renderpasses.cpp'],
  cpp_args: ['-DWLR_USE_UNSTABLE'],
  include_directories: incdir,
  # sometimes you need to add other hyprland dependencies yourself
  dependencies: [
    dependency('pixman-1'),
    dependency('libinput'),
    dependency('wayland-server'),
    dependency('xkbcommon'),
    dependency('libdrm'),
    hyprland_headers
  ],
  install: true)
