cmake_minimum_required(VERSION 4.0)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL test.cpp main.cpp)

target_include_directories(OpenGL PRIVATE dependencies/include)

target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw/libglfw3.a "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)