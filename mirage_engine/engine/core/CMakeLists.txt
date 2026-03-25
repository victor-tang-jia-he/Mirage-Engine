# Get all files in the Source and include directories
file(GLOB_RECURSE CORE_SOURCES 
     "source/*.cpp"
     "source/*.c"
     "source/Components/*.c"
     "source/Components/*.cpp"
)

file(GLOB_RECURSE CORE_HEADERS
     "include/*.h"
     "include/*.hpp"
     "include/Components/*.h"
     "include/Components/*.hpp"
)
add_library(Core 

    ${CORE_SOURCES}
    ${CORE_HEADERS})
target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Components
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Components
)

target_link_libraries(Core PRIVATE third_party)


target_compile_features(Core INTERFACE cxx_std_17)