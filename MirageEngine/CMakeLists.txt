# CMakeList.txt : CMake project for MirageEngine, include source and define
# project specific logic here.
#



# CMake Version
cmake_minimum_required(VERSION 3.16)

# Project Name
project(CustomEngine)

# Add source to this project's execumle.
add_executable(MirageEngine "main.cpp")

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
add_subdirectory(glfw)
add_subdirectory(Tools/Toolbox3D)

# Add glfw dependency to target
target_link_libraries(MirageEngine PRIVATE glfw Toolbox)