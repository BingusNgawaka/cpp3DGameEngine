# Minimum required version of CMake
cmake_minimum_required(VERSION 3.24)

# Project name and version
project(main VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Package preferences
set(OpenGL_GL_PREFERENCE "GLVND")

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include the directories of the found packages and own headers
include_directories(${CMAKE_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

# source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add your source files here 
add_executable(main ${SOURCES})

# Link against the required libraries
target_link_libraries(main ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} glm::glm)

