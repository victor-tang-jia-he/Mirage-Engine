# Project executable
add_executable(MirageEngineEditor)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_SOURCES
    "Source/Src/*.cpp"
    "Source/Src/*.c"
)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_HEADERS
    "Source/Include/*.hpp"
    "Source/Include/*.h"
)

target_sources(MirageEngineEditor PRIVATE ${MIRAGE_ENGINE_EDITOR_SOURCES} ${MIRAGE_ENGINE_EDITOR_HEADERS} "Source/Include/main_menu_bar.h" "Source/Src/main_menu_bar.cpp" "Source/Include/scene_graph.h" "Source/Src/scene_graph.cpp" "Source/Include/dragdrop.h" "Source/Src/dragdrop.cpp")

# Include directories
target_include_directories(MirageEngineEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Include
)

target_compile_definitions(MirageEngineEditor PRIVATE 
    ASSETS_PATH="${CMAKE_SOURCE_DIR}/MirageEngine/Assets"
)

# Link engine and third-party
target_link_libraries(MirageEngineEditor PRIVATE
    MirageEngine    # Link the engine itself
    third_party     # And third-party libs
)
