set(
    BENCHMARK_SRC
    main.cpp
)

add_executable(spritebenchmark ${BENCHMARK_SRC})

target_link_libraries(
    spritebenchmark
    PRIVATE SDL3::SDL3
    util math input render app
)

install(TARGETS spritebenchmark DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        spritebenchmark
        PROPERTIES LINK_FLAGS "-sENVIRONMENT=web -sMODULARIZE=instance -sFULL_ES3"
    )
endif()
