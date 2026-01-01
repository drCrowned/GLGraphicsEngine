cmake_minimum_required(VERSION 3.20)
project(OpenGLEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
)

add_executable(OpenGLEngine ${SOURCES})

# Include directories
target_include_directories(OpenGLEngine PRIVATE
    Libraries/include
)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR})

# GLFW
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(OpenGLEngine PRIVATE glfw)

# Platform-specific OpenGL
if (APPLE)
    target_link_libraries(OpenGLEngine PRIVATE "-framework OpenGL")
elseif (WIN32)
    target_link_libraries(OpenGLEngine PRIVATE opengl32)
elseif (UNIX)
    target_link_libraries(OpenGLEngine PRIVATE GL)
endif()
