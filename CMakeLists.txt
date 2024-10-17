cmake_minimum_required(VERSION 3.0)
project(NBodySimulation)

set(CMAKE_CXX_STANDARD 11)

# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Include directories for GLAD, GLM, and other headers
include_directories(include)
include_directories(/path/to/glm)  # Update this if GLM is installed in a custom path

# Add the source files and executable
add_executable(n_body_simulation src/main.cpp src/Body.cpp src/Vector.cpp src/Simulation.cpp src/glad.c)

# Link libraries (OpenGL and GLFW)
target_link_libraries(n_body_simulation glfw OpenGL::GL)

# macOS-specific frameworks
if(APPLE)
    target_link_libraries(n_body_simulation "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# Target include directories for GLM
target_include_directories(n_body_simulation PRIVATE include /opt/homebrew/opt/glm)
