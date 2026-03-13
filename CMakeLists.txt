# CMake version
cmake_minimum_required(VERSION 3.16)

# Project name
project(CustomEngine)

# Create global variable (project-level) to store output directory
set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
# Log directory at configuration-time
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MY_BIN_OUTPUT_DIR}")

# Set output directory for compiled libs and exe (project-level)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}")

# Set C standard version
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Adding external libraries...")

# ============================================================================
# GLFW
# ============================================================================
# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
# option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" ${GLFW_STANDALONE})
# option(GLFW_BUILD_TESTS "Build the GLFW test programs" ${GLFW_STANDALONE})
# option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)
# option(GLFW_INSTALL "Generate installation target" ON)
add_subdirectory(libraries/GLFW GLFW)

# ============================================================================
# GLAD (OpenGL loader) - needed for ImGui OpenGL backend
# ============================================================================
# Check if glad has its own CMakeLists
if(EXISTS ${CMAKE_SOURCE_DIR}/libraries/glad/CMakeLists.txt)
    add_subdirectory(libraries/glad glad)
else()
    # Create simple glad target
    if(EXISTS ${CMAKE_SOURCE_DIR}/libraries/glad/src/glad.c)
        add_library(glad STATIC libraries/glad/src/glad.c)
        target_include_directories(glad PUBLIC libraries/glad/include)
    else()
        message(WARNING "glad.c not found! Please generate glad files first.")
    endif()
endif()

# ============================================================================
# ImGUI - Manual configuration (since it lacks CMakeLists.txt)
# ============================================================================
# Collect all ImGui source files including backends
file(GLOB IMGUI_CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/libraries/imgui/*.cpp
)

file(GLOB IMGUI_BACKEND_SOURCES
    ${CMAKE_SOURCE_DIR}/libraries/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/libraries/imgui/backends/imgui_impl_opengl3.cpp
)

# Combine all sources
set(IMGUI_ALL_SOURCES ${IMGUI_CORE_SOURCES} ${IMGUI_BACKEND_SOURCES})

# Create ImGui library
add_library(imgui STATIC ${IMGUI_ALL_SOURCES})

# Add include directories
target_include_directories(imgui PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/imgui
    ${CMAKE_SOURCE_DIR}/libraries/imgui/backends
    ${CMAKE_SOURCE_DIR}/libraries/glad/include  # For glad headers
)
# Link dependencies to ImGui
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PRIVATE
    glfw          # Link GLFW to ImGui
    glad          # Link glad to ImGui
    OpenGL::GL    # Link OpenGL
)

# Tell ImGui we're using glad as OpenGL loader
target_compile_definitions(imgui PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# ============================================================================
# System Engine Library (Core)
# ============================================================================
add_subdirectory(${CMAKE_SOURCE_DIR}/mirage_engine/engine/core)
add_subdirectory(${CMAKE_SOURCE_DIR}/mirage_engine/tools/toolbox3D)

# Create Convenience Interface for All Dependencies
add_library(third_party INTERFACE)

target_include_directories(third_party INTERFACE
    ${CMAKE_SOURCE_DIR}/libraries/glad/include
    ${CMAKE_SOURCE_DIR}/libraries/GLFW/include
    ${CMAKE_SOURCE_DIR}/libraries/imgui
    ${CMAKE_SOURCE_DIR}/libraries/imgui/backends
)

target_link_libraries(third_party INTERFACE
    glfw
    glad
    imgui
    OpenGL::GL
    Toolbox
    Core
)

# Engine project
message(STATUS "Adding mirage_engine project...")
add_subdirectory(mirage_engine)

# Editor project
message(STATUS "Adding mirage_engine_editor project...")
add_subdirectory(mirage_engine_editor)

# Final Log
message(STATUS "")
message(STATUS "========================================")
message(STATUS "${PROJECT_NAME} Configuration Complete")
message(STATUS "========================================")
message(STATUS "Build Directory: ${MY_BIN_OUTPUT_DIR}")
# message(STATUS "Build Engine: ${BUILD_ENGINE}")
# message(STATUS "Build Editor: ${BUILD_EDITOR}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "========================================")