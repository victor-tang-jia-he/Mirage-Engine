# Get all files in the Source and include directories
file(GLOB_RECURSE CORE_SOURCES 
     "source/*.cpp"
     "source/*.c"
)

file(GLOB_RECURSE CORE_HEADERS
     "include/*.h"
     "include/*.hpp"
)
add_library(Core STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS})
target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_compile_features(Core INTERFACE cxx_std_17)