project('wigglewobble', 'cpp', 'c',
  # we always take the VERSION file as the single source of truth
  version: run_command('cat', join_paths(meson.project_source_root(), 'VERSION'), check: true).stdout().strip(),
  default_options: ['buildtype=release'],
)

cpp_compiler = meson.get_compiler('cpp')
if cpp_compiler.has_argument('-std=c++26')
  add_global_arguments('-std=c++26', language: 'cpp')
elif cpp_compiler.has_argument('-std=c++2c')
  add_global_arguments('-std=c++2c', language: 'cpp')
else
  error('Could not configure current C++ compiler (' + cpp_compiler.get_id() + ' ' + cpp_compiler.version() + ') with required C++ standard (C++26)')
endif

hyprland_headers = dependency('hyprland')

incdir = include_directories('include')

subdir('include')
subdir('src')
