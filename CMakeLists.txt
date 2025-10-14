cmake_minimum_required(VERSION 3.12)
project(OpenGL)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# find glfw through cmake
find_package(glfw3 3.3 REQUIRED)

# find OpenAL-soft through cmake
find_package(OpenAL REQUIRED PATHS "/usr/local/Cellar/openal-soft/1.24.3/lib/cmake" NO_DEFAULT_PATH)

# finds all files in the project that end in .cpp
# CONFIGURE_DEPENDS says to re-find if a file is removed/added/moved/renamed/etc.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.cpp")

# we dont wanna include any files in the dependencies/tests/res folders
list(FILTER SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/(tests|res|cmake-build-debug|cmake-build-release)/.*")

# main executable
add_executable(OpenGL ${SOURCES})

# add include directory
target_include_directories(OpenGL PRIVATE dependencies/include)

# link with glad from our dependencies folder. cross platform compatible
target_link_libraries(OpenGL PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib/glad/libglad.a)

# link with glfw
target_link_libraries(OpenGL PRIVATE glfw)

# link with OpenAL
target_link_libraries(OpenGL PRIVATE OpenAL::OpenAL)

enable_testing()

add_executable(NanoIdTests tests/nanoIdTests.cpp idGenerators.cpp)

add_test(NAME NanoIdTests COMMAND NanoIdTests)