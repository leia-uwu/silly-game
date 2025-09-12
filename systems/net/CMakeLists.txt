set(
    NET_SRC
    bytestream.cpp
)

add_library(net STATIC ${NET_SRC})
set_target_properties(
    net
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)
