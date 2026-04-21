cmake_minimum_required(VERSION 3.16)
project(Toolbox)

include(FetchContent)

# ============================================================================
# Options
# ============================================================================

option(TOOLBOX_BUILD_TESTS   "Build tests"                    OFF)
option(TOOLBOX_FETCH_GTEST   "Download GoogleTest if missing" OFF)
option(TOOLBOX_INTEG_GLM     "Add GLM to the project"         OFF)

# ============================================================================
# Dependencies
# ============================================================================

# GLM (header-only)
set(GLM_LOCAL_PATH "${CMAKE_SOURCE_DIR}/Libraries/glm")
if (TOOLBOX_INTEG_GLM)
    if(EXISTS "${GLM_LOCAL_PATH}/CMakeLists.txt")
        message(STATUS "GLM: using local copy")
        add_subdirectory(${GLM_LOCAL_PATH} ${CMAKE_BINARY_DIR}/glm)
    else()
        message(STATUS "GLM: fetching from remote...")
        FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG        1.0.3
            SOURCE_DIR     ${GLM_LOCAL_PATH}
        )
        FetchContent_MakeAvailable(glm)
    endif()
endif()

# ============================================================================
# Toolbox library
# ============================================================================

file(GLOB_RECURSE TOOLBOX_SOURCES "Source/Src/*.cpp" "Source/Src/*.c")
file(GLOB_RECURSE TOOLBOX_HEADERS "Source/Include/*.hpp" "Source/Include/*.h")

add_library(Toolbox STATIC ${TOOLBOX_SOURCES} ${TOOLBOX_HEADERS})

target_include_directories(Toolbox
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source
)
if (TOOLBOX_INTEG_GLM)
    target_link_libraries(Toolbox PUBLIC glm::glm)
endif()

target_compile_features(Toolbox INTERFACE cxx_std_17)

# ============================================================================
# Tests
# ============================================================================

if(NOT TOOLBOX_BUILD_TESTS)
    return()
endif()

message(STATUS "Tests: enabled")

set(GTEST_LOCAL_PATH "${CMAKE_SOURCE_DIR}/Libraries/googletest")
set(GTEST_CMAKE_OPTIONS
    gtest_force_shared_crt ON  CACHE BOOL "" FORCE
    BUILD_GMOCK            ON  CACHE BOOL "" FORCE
    INSTALL_GTEST          OFF CACHE BOOL "" FORCE
)

if(EXISTS "${GTEST_LOCAL_PATH}/CMakeLists.txt")
    message(STATUS "GoogleTest: using local copy")
    set(gtest_force_shared_crt ON  CACHE BOOL "" FORCE)
    set(BUILD_GMOCK            ON  CACHE BOOL "" FORCE)
    set(INSTALL_GTEST          OFF CACHE BOOL "" FORCE)
    add_subdirectory(${GTEST_LOCAL_PATH} ${CMAKE_BINARY_DIR}/googletest)

elseif(TOOLBOX_FETCH_GTEST)
    message(STATUS "GoogleTest: fetching from remote...")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON  CACHE BOOL "" FORCE)
    set(BUILD_GMOCK            ON  CACHE BOOL "" FORCE)
    set(INSTALL_GTEST          OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

else()
    message(WARNING "GoogleTest not found and TOOLBOX_FETCH_GTEST is OFF ¡ª skipping tests.")
    return()
endif()

add_subdirectory(tests)