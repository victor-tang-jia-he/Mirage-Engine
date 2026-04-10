cmake_minimum_required(VERSION 3.16)
project(Toolbox)

option(TOOLBOX_BUILD_TESTS "Build tests" OFF)
option(TOOLBOX_FETCH_GTEST "Download GoogleTest if missing" OFF)

# Tests
if(TOOLBOX_BUILD_TESTS)
    include(FetchContent)

    # GLM (header-only library)
    if(EXISTS "${CMAKE_SOURCE_DIR}/Libraries/glm" AND EXISTS "${CMAKE_SOURCE_DIR}/Libraries/glm/CMakeLists.txt")
        message(STATUS "Using local GLM")
        add_subdirectory(${CMAKE_SOURCE_DIR}/Libraries/glm ${CMAKE_BINARY_DIR}/glm)
    else()
        message(STATUS "Fetching GLM...")
        FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/Libraries/glm
        )
        FetchContent_MakeAvailable(glm)
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

    message(STATUS "Building tests...")

    set(GTEST_PATH ${CMAKE_SOURCE_DIR}/Libraries/googletest)

    if(EXISTS ${GTEST_PATH}/CMakeLists.txt)
        message(STATUS "Using local GoogleTest")

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

        add_subdirectory(${GTEST_PATH} ${CMAKE_BINARY_DIR}/googletest)

        add_subdirectory(tests)
    else()
        message(WARNING "GoogleTest not found. .")
        if(TOOLBOX_FETCH_GTEST)
            message(STATUS "Fetching GoogleTest...")
            include(FetchContent)

            FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG v1.14.0
            )

            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
            set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
            set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

            FetchContent_MakeAvailable(googletest)

            add_subdirectory(tests)
        else()
            message(WARNING "GoogleTest not found and TOOLBOX_FETCH_GTEST is OFF. Tests will not be built.")
        endif()
    endif()
endif()