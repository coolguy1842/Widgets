project('widgets', 'cpp', default_options : ['cpp_std=c++23'])
fs = import('fs')

libgtkwidgetspp_proj = subproject('gtkwidgetspp')
libgtkwidgetspp_dep = libgtkwidgetspp_proj.get_variable('libgtkwidgetspp_dep')

executable(
    'widgets',
    # sources : [ files('src/Utils/CSSUtil.cpp', 'src/Utils/PathUtil.cpp', 'src/Utils/ProcessUtil.cpp', 'src/Widgets/Widget.cpp', 'src/Widgets/Window.cpp', 'src/main.cpp') ],
    sources: [ files('src/main.cpp') ],
    include_directories: include_directories('include'),
    dependencies: [
        dependency('gtk4'),
        dependency('gtkmm-4.0'),
        dependency('gtk4-layer-shell-0'),
        dependency('nlohmann_json'),
        dependency('fmt'),
        dependency('CLI11'),
        libgtkwidgetspp_dep
    ],
    install: false
)