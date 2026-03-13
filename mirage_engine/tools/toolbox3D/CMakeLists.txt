cmake_minimum_required(VERSION 3.16)
project(Toolbox)

option(TOOLBOX_BUILD_TESTS "Build tests" ON)

include(FetchContent)

# GLM (header-only library)
message(STATUS "Fetching GLM...")
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

# Check GLM
if(NOT glm_POPULATED)
    message(FATAL_ERROR "GLM failed to populate")
endif()

# Main Toolbox
file(GLOB_RECURSE TOOLBOX_SOURCES 
     "Source/src/*.cpp"
     "Source/src/*.c"
)

file(GLOB_RECURSE TOOLBOX_HEADERS
     "Source/include/*.hpp"
     "Source/include/*.h"
)
add_library(Toolbox STATIC
    ${TOOLBOX_SOURCES}
    ${TOOLBOX_HEADERS})
target_include_directories(Toolbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(Toolbox
    PUBLIC
        glm::glm
)

target_compile_features(Toolbox INTERFACE cxx_std_17)

# Tests
if(TOOLBOX_BUILD_TESTS)
    message(STATUS "Building tests...")
    
    # GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    
    # GoogleTest Options
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
endif()