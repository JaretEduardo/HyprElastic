project('HyprElastic', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=release',
    'warning_level=2'
  ]
)

add_project_arguments('-DWLR_USE_UNSTABLE', language: 'cpp')

hyprland = dependency('hyprland')
pixman = dependency('pixman-1')
libdrm = dependency('libdrm')

inc = include_directories('include')

shared_module('hyprelastic',
  'src/main.cpp',
  'src/WobblyWindow.cpp',
  'src/SpringPhysics.cpp',
  'src/RenderWobbly.cpp',
  include_directories: inc,
  dependencies: [hyprland, pixman, libdrm],
  install: true,
)