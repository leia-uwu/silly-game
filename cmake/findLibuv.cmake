function(findLibuv)
    FetchContent_Declare(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv
        GIT_TAG        v1.51.0
        GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(libuv)
    add_library(libuv ALIAS uv_a)
endfunction()
