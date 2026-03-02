# CMakeList.txt : CMake project for MirageEngine, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.16)

project(MirageEngine)

# Add source to this project's executable.
add_executable (MirageEngine "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MirageEngine PROPERTY CXX_STANDARD 20)
endif()

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
target_compile_definitions(MirageEngine PRIVATE USE_RHI_VULKAN=1)