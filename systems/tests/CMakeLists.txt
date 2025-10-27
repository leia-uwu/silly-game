add_executable(ByteStreamTest bytestreamTest.cpp)
target_link_libraries(ByteStreamTest net math)

add_test(
    NAME ByteStreamTest
    COMMAND ByteStreamTest
)

add_executable(GridTest gridTest.cpp)
target_link_libraries(GridTest math)

add_test(
    NAME GridTest
    COMMAND GridTest
)
