project('widgets', 'cpp', default_options : ['cpp_std=c++23'])

c = run_command('get_srcfiles.sh', check: true)
sources = c.stdout().strip().split('\n')

executable(
    'widgets',
    sources,
    include_directories: include_directories('include'),
    dependencies: [
        dependency('gtk4'),
        dependency('gtk4-layer-shell-0'),
        dependency('nlohmann_json'),
        dependency('fmt'),
        dependency('CLI11')
    ],
    install: false
)