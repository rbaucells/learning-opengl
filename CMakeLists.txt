cmake_minimum_required(VERSION 4.0)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL components/RotateComponent.cpp components/RotateComponent.h components/component.h math/columnMatrix4x4.cpp math/mathematics.h math/rowMatrix4x4.cpp math/vector2.cpp math/vector3.cpp Camera.cpp main.cpp main.h object.cpp object.h camera.h dependencies/stb_image.cpp
        input.h
        input.cpp
        Event.h
        components/Renderer.cpp
        components/Renderer.h
        transform.cpp
        math/vector4.cpp
        workQueue.h
        workQueue.cpp
)

target_include_directories(OpenGL PRIVATE dependencies/include)

target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw/libglfw3.a "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)

enable_testing()

#add_executable(MathTests math/columnMatrix4x4.cpp math/mathematics.h tests/MathTests.cpp dependencies/include/linmath/linmath.h)
#
#target_include_directories(MathTests PRIVATE dependencies/include)
#
#add_test(NAME MathTests COMMAND MathTests)

add_executable(transformTests tests/transformTests.cpp math/mathematics.h dependencies/include/linmath/linmath.h transform.cpp math/columnMatrix4x4.cpp math/vector2.cpp)

target_include_directories(transformTests PRIVATE dependencies/include)

add_test(NAME transformTests COMMAND transformTests)