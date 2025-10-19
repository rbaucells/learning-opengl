cmake_minimum_required(VERSION 3.12)
project(learningOpenGl)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)

# Add subdirectories for libraries
add_subdirectory(math++)
add_subdirectory(json++)
add_subdirectory(zero)

# Add executable
add_executable(myGame1 main.cpp)

# Link myGame1 to Zero
target_link_libraries(myGame1 PRIVATE Zero)