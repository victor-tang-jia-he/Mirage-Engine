file(GLOB_RECURSE MIRAGE_ENGINE_SCRIPTS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)
file(GLOB_RECURSE MIRAGE_ENGINE_SCRIPTS_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
)

add_library(GameScripts SHARED
    ${MIRAGE_ENGINE_SCRIPTS_SOURCES}
    ${MIRAGE_ENGINE_SCRIPTS_HEADERS}
)
set_target_properties(GameScripts PROPERTIES OUTPUT_NAME "GameScripts")
target_compile_definitions(GameScripts PRIVATE SCRIPTS_DLL_EXPORT CPP_TRACE_DEBUG)

target_include_directories(GameScripts PRIVATE
    ${CMAKE_SOURCE_DIR}/MirageEngine/Source/Include
)

target_link_libraries(GameScripts PRIVATE MirageEngine third_party)

set_target_properties(GameScripts
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${MIRAGE_BIN_OUTPUT_DIR}/DLL"
        RUNTIME_OUTPUT_DIRECTORY "${MIRAGE_BIN_OUTPUT_DIR}/DLL"
)

add_custom_command(TARGET GameScripts POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:GameScripts>
        $<TARGET_FILE_DIR:MirageEngineEditor>
)