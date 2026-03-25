# Project executable
add_executable(MirageEngineEditor)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_SOURCES
    "Source/*.cpp"
    "Source/*.c"
)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_HEADERS
    "Include/*.hpp"
    "Include/*.h"
)

target_sources(MirageEngineEditor PRIVATE ${MIRAGE_ENGINE_EDITOR_SOURCES} ${MIRAGE_ENGINE_EDITOR_HEADERS})

# Include directories
target_include_directories(MirageEngineEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_SOURCE_DIR}/mirage_engine/
)

target_compile_definitions(MirageEngineEditor PRIVATE 
    ASSETS_PATH="${CMAKE_SOURCE_DIR}/mirage_engine/assets"
)

# Link engine and third-party
target_link_libraries(MirageEngineEditor PRIVATE
    MirageEngine    # Link the engine itself
    third_party     # And third-party libs
)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngineEditor PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngineEditor PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()