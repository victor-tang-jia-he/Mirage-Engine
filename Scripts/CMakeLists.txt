file(GLOB_RECURSE MIRAGE_ENGINE_SCRIPTS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
file(GLOB_RECURSE MIRAGE_ENGINE_SCRIPTS_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(GameScripts SHARED
    ${MIRAGE_ENGINE_SCRIPTS_SOURCES}
    ${MIRAGE_ENGINE_SCRIPTS_HEADERS}
)

target_compile_definitions(GameScripts PRIVATE SCRIPTS_DLL_EXPORT)

target_include_directories(GameScripts PUBLIC
    ${CMAKE_SOURCE_DIR}/MirageEngine/Source/include
)
target_link_libraries(GameScripts PRIVATE
    third_party
)
add_custom_command(TARGET GameScripts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${MIRAGE_BIN_OUTPUT_DIR}/Libs"
        $<TARGET_FILE:GameScripts>
        $<TARGET_FILE_DIR:MirageEngine>
)