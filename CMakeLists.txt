cmake_minimum_required(VERSION 4.0)
project(OpenGL)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# find glfw through cmake
find_package(glfw3 3.3 REQUIRED)

find_package(OpenAL REQUIRED PATHS "/usr/local/Cellar/openal-soft/1.24.3/lib/cmake" NO_DEFAULT_PATH)
#find_package(OpenAL REQUIRED)

# main executable
add_executable(OpenGL components/componentExample.cpp components/componentExample.h systems/component.h math/vector2.cpp math/vector3.cpp components/camera.cpp main.cpp settings.h object.cpp object.h components/camera.h dependencies/stb_image.cpp
        systems/input.h
        systems/input.cpp
        components/renderer/renderer.h
        transform.cpp
        math/vector4.cpp
        systems/workQueue.h
        systems/workQueue.cpp
        systems/tweens/tween.h
        math/curve.cpp
        math/vector2.h
        math/vector3.h
        math/vector4.h
        math/vertex.h
        math/buffers.h
        math/matrix.h
        math/curve.h
        systems/component.cpp
        systems/tweens/sequenceTween.cpp
        systems/tweens/waitTween.cpp
        components/renderer/spriteRenderer.cpp
        components/renderer/customRenderer.cpp
        systems/tweens/callbackTween.cpp
        components/renderer/spriteSheetRenderer.cpp
        "systems/opengl wrappers/texture.cpp"
        "systems/opengl wrappers/texture.h"
        math/Color.h
        "systems/opengl wrappers/window.cpp"
        "systems/opengl wrappers/window.h"
        systems/audio/sound.cpp
        systems/audio/audioListener.cpp
        systems/audio/audioSource.cpp
        systems/audio/audioListener.cpp
        systems/audio/audioSource.cpp
        systems/audio/effects/frequencyShifter.cpp
        systems/audio/filters/lowPassFilter.cpp
        systems/audio/sound.h
        systems/audio/effects/audioEffect.h
        systems/audio/filters/audioFilter.h
        systems/audio/audioSource.h
        systems/audio/audioListener.h
        systems/audio/effects/eaxReverb.cpp
        systems/audio/effects/reverb.cpp
        systems/audio/effects/chorus.cpp
        systems/audio/effects/distortion.cpp
        systems/audio/effects/echo.cpp
        systems/audio/effects/flanger.cpp
        systems/audio/effects/vocalMorpher.cpp
        systems/audio/effects/pitchShifter.cpp
        systems/audio/effects/ringModulator.cpp
        systems/audio/effects/autowah.cpp
        systems/audio/effects/compressor.cpp
        systems/audio/effects/equalizer.cpp
        systems/audio/filters/highPassFilter.cpp
        systems/audio/filters/bandPassFilter.cpp
        math/bounds.h
        components/renderer/rendererBase.cpp
        systems/events.h
        "systems/opengl wrappers/ShaderProgram.cpp"
        "systems/opengl wrappers/ShaderProgram.h"
        "systems/opengl wrappers/Shader.cpp"
        "systems/opengl wrappers/Shader.h"
        exceptions/initializationException.h
)

# add include directory
target_include_directories(OpenGL PRIVATE dependencies/include)

target_link_libraries(OpenGL PRIVATE OpenAL::OpenAL)

# link with glad. cross platform compatible
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)

# link with glfw
target_link_libraries(OpenGL PRIVATE glfw)

enable_testing()

# Matrix Tests
add_executable(matrixTests tests/matrixTests.cpp)

target_include_directories(matrixTests PRIVATE dependencies/include)

add_test(NAME matrixTests COMMAND matrixTests)

# Transform Tests
add_executable(transformTests tests/transformTests.cpp transform.cpp math/vector2.cpp)

target_include_directories(transformTests PRIVATE dependencies/include)

add_test(NAME transformTests COMMAND transformTests)

# List Tests
add_executable(listTests tests/listTests.cpp list.h)

target_include_directories(listTests PRIVATE dependencies/include)

add_test(NAME listTests COMMAND listTests)

# Curve Tests
add_executable(curveTests tests/curveTests.cpp math/curve.cpp math/vector2.cpp math/curve.h math/vector2.cpp)

target_include_directories(curveTests PRIVATE)

add_test(NAME curveTests COMMAND curveTests)

add_executable(EventTests tests/eventTests.cpp systems/events.h)

add_test(NAME EventTests COMMAND EventTests)