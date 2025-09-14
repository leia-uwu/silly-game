function(findSDL)
    find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)
    if (NOT SDL3_FOUND)
        FetchContent_Declare(
            SDL
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG        release-3.2.22
            GIT_SHALLOW ON
        )
        FetchContent_MakeAvailable(SDL)
    endif()
endfunction()
