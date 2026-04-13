# CMake version
cmake_minimum_required(VERSION 3.20)

# Project name
project(CustomEngine)

# Create global variable (project-level) to store output directory
set(MIRAGE_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")

# Log directory at configuration-time
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MIRAGE_BIN_OUTPUT_DIR}")

# Set output directory for compiled libs and exe (project-level)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MIRAGE_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MIRAGE_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MIRAGE_BIN_OUTPUT_DIR}")

# Set C standard version
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Adding external Libraries...")
message("")

# Include Fetch system
include(FetchContent)

# ============================================================================
# GLFW
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "GLFW")

set(GLFW_SOURCE "${CMAKE_SOURCE_DIR}/Libraries/GLFW")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(NOT EXISTS "${GLFW_SOURCE}/CMakeLists.txt")
    message(STATUS "Fetching GLFW...")
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        SOURCE_DIR ${GLFW_SOURCE}
    )
    FetchContent_MakeAvailable(glfw)
    add_subdirectory(${GLFW_SOURCE} ${CMAKE_BINARY_DIR}/GLFW)
else()
    message(STATUS "Using local GLFW")
    add_subdirectory(${GLFW_SOURCE} ${CMAKE_BINARY_DIR}/GLFW)
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

message(STATUS "================================================================================")
message("")

# ============================================================================
# GLAD (OpenGL loader)
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "GLAD")

if(EXISTS ${CMAKE_SOURCE_DIR}/Libraries/Glad/Src/Glad/glad.c)
    add_library(glad STATIC ${CMAKE_SOURCE_DIR}/Libraries/Glad/Src/Glad/glad.c)
    target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/Libraries/Glad/Include)
else()
    message(WARNING "glad.c not found! Please generate glad files first.")
endif()

message(STATUS "GLAD target created successfully")

message(STATUS "================================================================================")
message("")

# ============================================================================
# ImGUI
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "IMGUI")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/Libraries/ImGui")
set(IMGUI_BACKENDS_DIR "${CMAKE_SOURCE_DIR}/Libraries/ImGui/backends")

file(GLOB IMGUI_CONTENTS ${IMGUI_DIR}/*)
list(LENGTH IMGUI_CONTENTS IMGUI_CONTENT_COUNT)

# Check if ImGui folder is empty
if(NOT EXISTS ${IMGUI_DIR} OR IMGUI_CONTENT_COUNT EQUAL 0)
    message(STATUS "Fetching ImGui...")
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
        SOURCE_DIR ${IMGUI_DIR}
    )
    FetchContent_MakeAvailable(imgui)

    if(NOT imgui_POPULATED)
        message(FATAL_ERROR "ImGui failed to populate")
    endif()
else()
    message(STATUS "Using local ImGui from ${IMGUI_DIR}")
endif()

set(imgui_SOURCE_DIR ${IMGUI_DIR})
message(STATUS "ImGui fetched successfully")

# Explicit list of ImGui core files (more reliable than GLOB)
set(IMGUI_CORE_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Backends
set(IMGUI_BACKEND_SOURCES
    ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

# Combine all sources
set(IMGUI_ALL_SOURCES
    ${IMGUI_CORE_SOURCES}
    ${IMGUI_BACKEND_SOURCES}
)

# Create the ImGui library
add_library(imgui STATIC ${IMGUI_ALL_SOURCES})

target_include_directories(imgui
    PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_BACKENDS_DIR}
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

message(STATUS "================================================================================")
message("")

# ============================================================================
# ImGuizmo
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "IMGUIZMO")

set(IMGUIZMO_DIR "${CMAKE_SOURCE_DIR}/Libraries/ImGuizmo")

file(GLOB IMGUIZMO_CONTENTS ${IMGUIZMO_DIR}/*)
list(LENGTH IMGUIZMO_CONTENTS IMGUIZMO_CONTENT_COUNT)

if(NOT EXISTS ${IMGUIZMO_DIR} OR IMGUIZMO_CONTENT_COUNT EQUAL 0)
    message(STATUS "Fetching ImGuizmo...")
    FetchContent_Declare(
        imguizmo
        GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo
        GIT_TAG master
        SOURCE_DIR ${IMGUIZMO_DIR}
    )
    FetchContent_MakeAvailable(imguizmo)
else()
    message(STATUS "Using local ImGuizmo")
endif()

set(IMGUIZMO_SOURCES
    ${IMGUIZMO_DIR}/ImGuizmo.cpp
)

# Combine all sources
set(IMGUIZMO_ALL_SOURCES
    ${IMGUIZMO_SOURCES}
)

# Create the ImGui library
add_library(imguizmo STATIC ${IMGUIZMO_ALL_SOURCES})
message(STATUS "ImGuizmo configured successfully")

message(STATUS "================================================================================")
message("")

# ============================================================================
# Tiny Object Loader
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "TINYOBJLOADER")

set(TINYOBJLOADER_DIR "${CMAKE_SOURCE_DIR}/Libraries/TinyObjLoader")

set(TINYOBJLOADER_CMAKE_DIR OFF CACHE BOOL "" FORCE)
set(TINYOBJLOADER_DOC_DIR OFF CACHE BOOL "" FORCE)
set(TINYOBJLOADER_INCLUDE_DIR OFF CACHE BOOL "" FORCE)
set(TINYOBJLOADER_LIBRARY_DIR OFF CACHE BOOL "" FORCE)
set(TINYOBJLOADER_PKGCONFIG_DIR OFF CACHE BOOL "" FORCE)
set(TINYOBJLOADER_RUNTIME_DIR OFF CACHE BOOL "" FORCE)

if(NOT EXISTS ${TINYOBJLOADER_DIR}/CMakeLists.txt)
    message(STATUS "Fetching TinyObjLoader...")
    FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v1.0.6
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/TinyObjLoader
    )
    FetchContent_MakeAvailable(tinyobjloader)
    add_subdirectory(${TINYOBJLOADER_DIR} ${CMAKE_BINARY_DIR}/TinyObjLoader EXCLUDE_FROM_ALL)
else()
    message(STATUS "Using local TinyObjLoader")
    add_subdirectory(${TINYOBJLOADER_DIR} ${CMAKE_BINARY_DIR}/TinyObjLoader EXCLUDE_FROM_ALL)
endif()

if(TARGET tinyobjloader)
    message(STATUS "TinyObjLoader target created successfully")
    get_target_property(TINYOBJLOADER_TYPE tinyobjloader TYPE)
    message(STATUS "TinyObjLoader target type: ${TINYOBJLOADER_TYPE}")
else()
    message(WARNING "TinyObjLoader target not found, checking for tinyobjloader::tinyobjloader...")
    if(TARGET tinyobjloader::tinyobjloader)
        message(STATUS "Found tinyobjloader::tinyobjloader target")
    else()
        message(FATAL_ERROR "No TinyObjLoader target found")
    endif()
endif()

message(STATUS "================================================================================")
message("")

# ============================================================================
# Bullet Physics
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "BULLET PHYSICS")

set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_PYBULLET OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
set(BUILD_ENET OFF CACHE BOOL "" FORCE)
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)

if(EXISTS "${CMAKE_SOURCE_DIR}/Libraries/Bullet3/CMakeLists.txt")
    message(STATUS "Using local Bullet3")
    add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/Bullet3 ${CMAKE_BINARY_DIR}/Bullet3)
else()
    message(STATUS "Fetching Bullet3...")
    FetchContent_Declare(
        bullet3
        GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/Bullet3
    )
    FetchContent_MakeAvailable(Bullet3)
endif()

message(STATUS "================================================================================")
message("")

# ============================================================================
# EnTTMeta Reflection
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "ENTT REFLECTION")

set(ENTT_DIR "${CMAKE_SOURCE_DIR}/Libraries/EnTT")

set(ENTT_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ENTT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ENTT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ENTT_INSTALL OFF CACHE BOOL "" FORCE)

if(NOT EXISTS ${ENTT_DIR}/CMakeLists.txt)
    message(STATUS "Fetching EnTT...")
    FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.16.0
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/EnTT
    )
    FetchContent_MakeAvailable(entt)
else()
    message(STATUS "Using local EnTT")
    add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/EnTT ${CMAKE_BINARY_DIR}/EnTT EXCLUDE_FROM_ALL)
endif()

if(TARGET entt)
    message(STATUS "EnTT target created successfully")
    get_target_property(ENTT_TYPE entt TYPE)
    message(STATUS "EnTT target type: ${ENTT_TYPE}")
endif()

message(STATUS "================================================================================")
message("")

# ============================================================================
# CPP Trace - Debug system
# ============================================================================
message(STATUS "================================================================================")
message(STATUS "CPP TRACE")

set(CPP_TRACE "${CMAKE_SOURCE_DIR}/Libraries/CppTrace")

set(CPP_TRACE_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(CPP_TRACE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPP_TRACE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CPP_TRACE_INSTALL OFF CACHE BOOL "" FORCE)

if(NOT EXISTS ${CPP_TRACE}/CMakeLists.txt)
    message(STATUS "Fetching CPP Trace...")
    FetchContent_Declare(
        cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG v1.0.4
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/CppTrace
    )
    FetchContent_MakeAvailable(cpptrace)
else()
    message(STATUS "Using local CPP Trace")
    add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/CppTrace ${CMAKE_BINARY_DIR}/CppTrace EXCLUDE_FROM_ALL)
endif()

message(STATUS "================================================================================")
message("")

# ============================================================================
# System Engine Library (Core)
# ============================================================================
add_subdirectory(${CMAKE_SOURCE_DIR}/MirageEngine/Tools/Toolbox)

# Create Convenience Interface for All Dependencies
add_library(third_party INTERFACE)

target_include_directories(third_party INTERFACE
    ${CMAKE_SOURCE_DIR}/Libraries/Glad/include
    ${CMAKE_SOURCE_DIR}/Libraries/GLFW/include
    ${CMAKE_SOURCE_DIR}/Libraries/ImGui
    ${CMAKE_SOURCE_DIR}/Libraries/ImGizmo
    ${CMAKE_SOURCE_DIR}/Libraries/ImGui/backends
    ${CMAKE_SOURCE_DIR}/Libraries/TinyObjLoader
    ${CMAKE_SOURCE_DIR}/Libraries/STBImage
    ${CMAKE_SOURCE_DIR}/Libraries/EnTT/single_include
    ${CMAKE_SOURCE_DIR}/Libraries/EnTT/src
    ${CMAKE_SOURCE_DIR}/Libraries/Bullet3/src
    ${CMAKE_SOURCE_DIR}/Libraries/CppTrace/include
    ${CMAKE_SOURCE_DIR}/Libraries
)

target_link_libraries(third_party INTERFACE
    MirageEngine
    glfw
    glad
    imgui
    tinyobjloader
    OpenGL::GL
    Toolbox
    BulletDynamics
    BulletCollision
    LinearMath
    MirageEngine
    cpptrace::cpptrace
) 

# Engine project
message("")
message(STATUS "Adding mirage_engine project...")
add_subdirectory(MirageEngine)

# Editor project
message("")
message(STATUS "Adding mirage_engine_editor project...")
add_subdirectory(MirageEngineEditor)

# Final Log
message(STATUS "")
message(STATUS "================================================================================")
message(STATUS "${PROJECT_NAME} Configuration Complete")
message(STATUS "================================================================================")
message(STATUS "Build Directory: ${MIRAGE_BIN_OUTPUT_DIR}")
# message(STATUS "Build Engine: ${BUILD_ENGINE}")
# message(STATUS "Build Editor: ${BUILD_EDITOR}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "================================================================================")