cmake_minimum_required(VERSION 3.12)
project(zero)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# find glfw through cmake
find_package(glfw3 3.3 REQUIRED)

# find OpenAL-soft through cmake
find_package(OpenAL REQUIRED PATHS "/usr/local/Cellar/openal-soft/1.24.3/lib/cmake" NO_DEFAULT_PATH)

add_library(zero STATIC
        src/application.cpp
        src/components/camera.cpp
        src/components/renderer/customRenderer.cpp
        src/components/renderer/rendererBase.cpp
        src/components/renderer/spriteRenderer.cpp
        src/components/renderer/spriteSheetRenderer.cpp
        src/nanoId.cpp
        src/object.cpp
        src/scene.cpp
        src/systems/audio/audioListener.cpp
        src/systems/audio/audioSource.cpp
        src/systems/audio/effects/autowah.cpp
        src/systems/audio/effects/chorus.cpp
        src/systems/audio/effects/compressor.cpp
        src/systems/audio/effects/distortion.cpp
        src/systems/audio/effects/eaxReverb.cpp
        src/systems/audio/effects/echo.cpp
        src/systems/audio/effects/equalizer.cpp
        src/systems/audio/effects/flanger.cpp
        src/systems/audio/effects/frequencyShifter.cpp
        src/systems/audio/effects/pitchShifter.cpp
        src/systems/audio/effects/reverb.cpp
        src/systems/audio/effects/ringModulator.cpp
        src/systems/audio/effects/vocalMorpher.cpp
        src/systems/audio/filters/bandPassFilter.cpp
        src/systems/audio/filters/highPassFilter.cpp
        src/systems/audio/filters/lowPassFilter.cpp
        src/systems/audio/sound.cpp
        src/systems/component.cpp
        src/systems/input.cpp
        src/systems/opengl-wrappers/shader.cpp
        src/systems/opengl-wrappers/shaderProgram.cpp
        src/systems/opengl-wrappers/texture.cpp
        src/systems/opengl-wrappers/window.cpp
        src/systems/tweens/callbackTween.cpp
        src/systems/tweens/sequenceTween.cpp
        src/systems/tweens/waitTween.cpp
        src/systems/workQueue.cpp
        src/transform.cpp
        src/serialization/componentRegistry.cpp
        dependencies/stb_image.cpp
)

# include dependencies
target_include_directories(zero PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include)

# add include directory publicly
target_include_directories(zero PUBLIC include)

# link with libraries
target_link_libraries(zero PUBLIC
        math++
        json++
)

# glad
target_link_libraries(zero PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/glad/libglad.a)

target_link_libraries(zero PUBLIC OpenAL::OpenAL glfw)