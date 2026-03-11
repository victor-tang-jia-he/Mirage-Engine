# CMake Version
cmake_minimum_required(VERSION 3.16)

# Project Name
project(CustomEngine)

# Add source to this project's executable.
add_executable (MirageEngineEditor "main.cpp" "Include/UIWrapper.h" "Source/UIWrapper.cpp")

# Set C standard version
target_compile_features(MirageEngineEditor PRIVATE c_std_11)

# Set C++ standard version
target_compile_features(MirageEngineEditor PRIVATE cxx_std_20)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
target_compile_options(MirageEngineEditor PRIVATE /W4 /WX)
else()
target_compile_options(MirageEngineEditor PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Add preprocessor definition
# target_compile_definitions(MirageEngineEditor PRIVATE USE_RHI_VULKAN=1)

# Search for CMakeLists.txt in glfw/ and add it to the build tree
# add_subdirectory(glfw)
# Add glfw dependency to target
# target_link_libraries(MirageEngineEditor PRIVATE glfw)