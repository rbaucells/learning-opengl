cmake_minimum_required(VERSION 3.12)
project(Zero)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# find glfw through cmake
find_package(glfw3 3.3 REQUIRED)

# find OpenAL-soft through cmake
find_package(OpenAL REQUIRED PATHS "/usr/local/Cellar/openal-soft/1.24.3/lib/cmake" NO_DEFAULT_PATH)

# finds all files in the project that end in .cpp
# CONFIGURE_DEPENDS says to re-find if a file is removed/added/moved/renamed/etc.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# main executable
add_library(Zero STATIC ${SOURCES})

# include dependencies
target_include_directories(Zero PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include)

# add include directory publicly
target_include_directories(Zero PUBLIC include)

# link with libraries
target_link_libraries(Zero PUBLIC
        Math++
        Json++
)

# glad
target_link_libraries(Zero PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/glad/libglad.a)

target_link_libraries(Zero PUBLIC OpenAL::OpenAL glfw)