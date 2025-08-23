cmake_minimum_required(VERSION 4.0)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL components/moving.cpp components/moving.h components/component.h math/columnMatrix4x4.cpp math/mathematics.h math/rowMatrix4x4.cpp math/vector2.cpp math/vector3.cpp Camera.cpp main.cpp main.h object.cpp object.h camera.h dependencies/stb_image.cpp)

target_include_directories(OpenGL PRIVATE dependencies/include)

target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw/libglfw3.a "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)