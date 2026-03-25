# CMake version
cmake_minimum_required(VERSION 3.16)

# Project name
project(CustomEngine)

# Create global variable (project-level) to store output directory
set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
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

message(STATUS "Adding external Libraries...")

# Include Fetch system
include(FetchContent)

# ============================================================================
# GLFW
# ============================================================================

if(EXISTS "${CMAKE_SOURCE_DIR}/Libraries/GLFW" AND EXISTS "${CMAKE_SOURCE_DIR}/Libraries/GLFW/CMakeLists.txt")
    message(STATUS "Using local GLFW")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build documentation" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/GLFW ${CMAKE_BINARY_DIR}/GLFW)
else()
    message(STATUS "Fetching GLFW...")
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/GLFW
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build documentation" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

if(TARGET glfw)
    message(STATUS "GLFW target created successfully")
    get_target_property(GLFW_TYPE glfw TYPE)
    message(STATUS "GLFW target type: ${GLFW_TYPE}")
else()
    message(WARNING "GLFW target not found, checking for glfw::glfw...")
    if(TARGET glfw::glfw)
        message(STATUS "Found glfw::glfw target")
    else()
        message(FATAL_ERROR "No GLFW target found")
    endif()
endif()

# ============================================================================
# GLAD (OpenGL loader) - needed for ImGui OpenGL backend
# ============================================================================
# Check if glad has its own CMakeLists
if(EXISTS ${CMAKE_SOURCE_DIR}/Libraries/glad/CMakeLists.txt)
    add_subdirectory(Libraries/glad glad)
else()
    # Create simple glad target
    if(EXISTS ${CMAKE_SOURCE_DIR}/Libraries/glad/src/glad.c)
        add_library(glad STATIC Libraries/glad/src/glad.c)
        target_include_directories(glad PUBLIC Libraries/glad/include)
    else()
        message(WARNING "glad.c not found! Please generate glad files first.")
    endif()
endif()

# ============================================================================
# ImGUI - Manual configuration (since it lacks CMakeLists.txt)
# ============================================================================

file(GLOB RESULT ${CMAKE_SOURCE_DIR}/Libraries/imgui/*)
list(LENGTH RESULT RES_LEN)

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/Libraries/imgui OR RES_LEN EQUAL 0)
    message(STATUS "Fetching ImGui...")
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/imgui
    )
    FetchContent_MakeAvailable(imgui)

    if(NOT imgui_POPULATED)
        message(FATAL_ERROR "ImGui failed to populate")
    endif()
else()
    message(STATUS "Using local ImGui")
endif()

message(STATUS "ImGui fetched successfully")
set(imgui_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/imgui)


# Explicit list of ImGui core files (more reliable than GLOB)
set(IMGUI_CORE_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

# Backends
set(IMGUI_BACKEND_SOURCES
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# ImGUI miscs
# set(IMGUI_EXTRA_SOURCES
#     ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
# )

# Combine all sources
set(IMGUI_ALL_SOURCES
    ${IMGUI_CORE_SOURCES}
    ${IMGUI_BACKEND_SOURCES}
    # ${IMGUI_EXTRA_SOURCES}
)

# Create the ImGui library
add_library(imgui STATIC ${IMGUI_ALL_SOURCES})

# Properties to improve compilation times
target_compile_features(imgui PRIVATE cxx_std_11)

target_include_directories(imgui
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# Linking dependencies
find_package(OpenGL REQUIRED)
target_link_libraries(imgui
    PUBLIC
        glfw          # GLFW must be PUBLIC because it is used in the backend headers
        OpenGL::GL    # OpenGL is also public
    PRIVATE           
        glad          # glad can remain private if used only in .cpp files
)

# Definitions for ImGui
target_compile_definitions(imgui
    PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLAD 
)

# Optional: Create an alias for clarity
add_library(ImGui::imgui ALIAS imgui)

# ============================================================================
# System Engine Library (Core)
# ============================================================================
add_subdirectory(${CMAKE_SOURCE_DIR}/mirage_engine/engine/core)
add_subdirectory(${CMAKE_SOURCE_DIR}/mirage_engine/tools/toolbox3D)

# Create Convenience Interface for All Dependencies
add_library(third_party INTERFACE)

target_include_directories(third_party INTERFACE
    ${CMAKE_SOURCE_DIR}/Libraries/glad/include
    ${CMAKE_SOURCE_DIR}/Libraries/GLFW/include
    ${CMAKE_SOURCE_DIR}/Libraries/imgui
    ${CMAKE_SOURCE_DIR}/Libraries/imgui/backends
    ${CMAKE_SOURCE_DIR}/Libraries
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