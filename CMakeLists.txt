cmake_minimum_required(VERSION 4.0)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL main.cpp
        main.h
        dependencies/stb_image.cpp
        Object.cpp
        math/MATH.h
        Camera.cpp
        components/Component.h
        components/collider.cpp
        components/collider.h
        math/Vector2.cpp
        math/Vector3.cpp
        math/matrix4x4.cpp)

target_include_directories(OpenGL PRIVATE dependencies/include)

target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw/libglfw3.a "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)