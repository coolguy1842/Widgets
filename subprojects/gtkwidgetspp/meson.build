project('gtkwidgetspp', 'cpp', default_options : ['cpp_std=c++23'])

inc = include_directories('include')
libgtkwidgetspp = static_library(
    'gtkwidgetspp',
    sources: [
        files(
            'src/Utils/CSSUtil.cpp',
            'src/Utils/PathUtil.cpp',
            'src/Utils/ProcessUtil.cpp',
            'src/Utils/StringUtil.cpp',

            'src/Widgets/Widget.cpp',
            'src/Widgets/Label.cpp',
            'src/Widgets/Icon.cpp',
            'src/Widgets/Button.cpp',
            'src/Widgets/Box.cpp',
            'src/Widgets/CenterBox.cpp',
            'src/Widgets/Window.cpp',

            'src/Services/Hyprservice/client.cpp',
            'src/Services/Hyprservice/workspace.cpp',
            'src/Services/Hyprservice/monitor.cpp',
            'src/Services/Hyprservice/actives.cpp',
            'src/Services/Hyprservice/socket.cpp',
            'src/Services/Hyprservice/event.cpp',
            'src/Services/Hyprservice/service.cpp'
        )
    ],
    include_directories : inc,
    install : false,
    dependencies: [
        dependency('gtk4'),
        dependency('gtkmm-4.0'),
        dependency('gtk4-layer-shell-0'),
        dependency('nlohmann_json'),
    ]
)

libgtkwidgetspp_dep = declare_dependency(
    include_directories : inc,
    link_with : libgtkwidgetspp,
    dependencies: [
        dependency('gtk4'),
        dependency('gtkmm-4.0'),
        dependency('gtk4-layer-shell-0'),
        dependency('nlohmann_json'),
    ]
)
