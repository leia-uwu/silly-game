project(
    'silly-game',
    'c',
    'cpp',
    default_options: ['cpp_std=c++23'],
)

cmake = import('cmake')

sdl3_dep = dependency(
    'sdl3',
    required: false,
)

if not sdl3_dep.found()
    opt_var = cmake.subproject_options()
    opt_var.set_install(false)
    sdl3_subproject = cmake.subproject('SDL', options: opt_var)
    sdl3_dep = sdl3_subproject.dependency('SDL3-static')
endif

subdir('common')
subdir('client')
subdir('server')

libcommon = static_library(
    'libcommon',
    common_files,
)

clientexe = executable(
    'game_client',
    client_files,
    link_with: libcommon,
    # wasm build has custom install target to install it to client public directory
    install: host_machine.system() != 'emscripten',
    dependencies: sdl3_dep,
    cpp_args: ['-DSDL=true'],
)

if host_machine.system() == 'emscripten'
    install_data(
        [
            meson.build_root() + '/game_client.wasm',
            meson.build_root() + '/game_client.js',
        ],
        install_dir: meson.source_root() + '/client/public',
    )
else
    run_target('client', command: clientexe)

    serverexe = executable(
        'game_server',
        server_files,
        link_with: libcommon,
    )

    run_target('server', command: serverexe)
endif
