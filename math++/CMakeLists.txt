cmake_minimum_required(VERSION 3.12)
project(math++)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# finds all files in the lib that end in .cpp
# CONFIGURE_DEPENDS says to re-find if a file is removed/added/moved/renamed/etc.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# math++ lib
add_library(math++ STATIC ${SOURCES})

target_include_directories(math++ PUBLIC include)