add_executable(ByteStreamTest bytestreamTest.cpp)
target_link_libraries(ByteStreamTest net math)

add_test(
    NAME ByteStreamTest
    COMMAND ByteStreamTest
)
