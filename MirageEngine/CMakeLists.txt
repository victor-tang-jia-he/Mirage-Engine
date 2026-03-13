# CMake Version
cmake_minimum_required(VERSION 3.16)

# Project Name
project(CustomEngine)

# Create global variable (project-level) to store output directory
set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
# Log directory at configuration-time
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MY_BIN_OUTPUT_DIR}")

# Set output directory for compiled libs and exe (project-level)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}")

# Add source to this project's executable.
add_executable (MirageEngine "main.cpp" "MirageRHI/src/test_main.cpp")

# Set C standard version
target_compile_features(MirageEngine PRIVATE c_std_11)

# Set C++ standard version
target_compile_features(MirageEngine PRIVATE cxx_std_20)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
target_compile_options(MirageEngine PRIVATE /W4 /WX)
else()
target_compile_options(MirageEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Add preprocessor definition
# target_compile_definitions(MirageEngine PRIVATE USE_RHI_VULKAN=1)

# Search for CMakeLists.txt in glfw/ and add it to the build tree
add_subdirectory(../Libraries/GLFW GLFW)

# Create directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/GLAD)

# Copy glad headers
file(
    COPY ${GLFW_SOURCE_DIR}/deps/glad
    DESTINATION ${CMAKE_BINARY_DIR}/include/
)

# Add to include paths
target_include_directories(
    MirageEngine PRIVATE
    ${CMAKE_BINARY_DIR}/include
    ${GLFW_SOURCE_DIR}/include
)

# Add glfw dependency to target
target_link_libraries(MirageEngine PRIVATE glfw)