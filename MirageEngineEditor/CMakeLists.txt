# Project executable
add_executable(MirageEngineEditor)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_SOURCES
    "Source/Src/*.cpp"
    "Source/Src/*.c"
    "${CMAKE_SOURCE_DIR}/libraries/ImGizmo/ImGuizmo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/ImGuizmo/ImGuizmo.cpp"
)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_HEADERS
    "Source/Include/*.hpp"
    "Source/Include/*.h"
)

target_sources(MirageEngineEditor PRIVATE ${MIRAGE_ENGINE_EDITOR_SOURCES} ${MIRAGE_ENGINE_EDITOR_HEADERS})

# Include directories
target_include_directories(MirageEngineEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Include
    ${CMAKE_SOURCE_DIR}/Libraries/ImGuizmo
)

# Link engine and third-party
target_link_libraries(MirageEngineEditor PRIVATE
    MirageEngine    # Link the engine itself
    third_party     # And third-party libs
    imguizmo
    GameScripts
)