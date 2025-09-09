cmake_minimum_required(VERSION 4.0)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL components/rotateComponent.cpp components/rotateComponent.h systems/component.h math/vector2.cpp math/vector3.cpp components/camera.cpp main.cpp main.h object.cpp object.h components/camera.h dependencies/stb_image.cpp
        systems/input.h
        systems/input.cpp
        systems/event.h
        components/renderer.cpp
        components/renderer.h
        transform.cpp
        math/vector4.cpp
        systems/workQueue.h
        systems/workQueue.cpp
        systems/tween.h
        math/curve.cpp
        math/vector2.h
        math/vector3.h
        math/vector4.h
        math/vertex.h
        math/buffers.h
        math/matrix.h
        math/curve.h
        systems/sequence.cpp
        systems/sequence.h
        systems/component.cpp
)

target_include_directories(OpenGL PRIVATE dependencies/include)
target_include_directories(OpenGL PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw/libglfw3.a "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)

enable_testing()

add_executable(matrixTests tests/matrixTests.cpp)

target_include_directories(matrixTests PRIVATE dependencies/include)

add_test(NAME matrixTests COMMAND matrixTests)

add_executable(transformTests tests/transformTests.cpp transform.cpp math/vector2.cpp)

target_include_directories(transformTests PRIVATE dependencies/include)

add_test(NAME transformTests COMMAND transformTests)

add_executable(listTests tests/listTests.cpp list.h)

target_include_directories(listTests PRIVATE dependencies/include)

add_test(NAME listTests COMMAND listTests)

add_executable(curveTests tests/curveTests.cpp math/curve.cpp math/vector2.cpp math/curve.h math/vector2.cpp)

target_include_directories(curveTests PRIVATE)

add_test(NAME curveTests COMMAND curveTests)