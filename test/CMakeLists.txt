project(sandra-test LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

add_executable(hello-test hello-test.cpp)
target_link_libraries(hello-test sandra gtest gtest_main)