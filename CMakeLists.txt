cmake_minimum_required(VERSION 3.12)
project(LearningOpenGL)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# Add subdirectories for libraries
add_subdirectory(Math++)
add_subdirectory(Json++)
add_subdirectory(Zero)

# Add executable
add_executable(EntryPoint main.cpp)

# Link EntryPoint to Zero
target_link_libraries(EntryPoint PRIVATE Zero)