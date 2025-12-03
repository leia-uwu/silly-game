add_executable(grid_demo main.cpp)

target_link_libraries(
    grid_demo
    PRIVATE SDL3::SDL3
    util math input render app
)

install(TARGETS grid_demo DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        grid_demo
        PROPERTIES LINK_FLAGS "-sENVIRONMENT=web -sMODULARIZE=instance -sFULL_ES3"
    )
endif()
