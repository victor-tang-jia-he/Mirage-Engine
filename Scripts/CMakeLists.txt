file(GLOB_RECURSE MIRAGE_ENGINE_SCRIPTS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)
file(GLOB_RECURSE MIRAGE_ENGINE_SCRIPTS_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
)

set(SCRIPTS_LIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/scripts_sources.cmake")

file(WRITE "${SCRIPTS_LIST_FILE}" "set(GAME_SCRIPTS_SOURCES\n")

foreach(src ${MIRAGE_ENGINE_SCRIPTS_SOURCES})
    file(APPEND "${SCRIPTS_LIST_FILE}" "    \"${src}\"\n")
endforeach()

foreach(src ${MIRAGE_ENGINE_SCRIPTS_HEADERS})
    file(APPEND "${SCRIPTS_LIST_FILE}" "    \"${src}\"\n")
endforeach()

file(APPEND "${SCRIPTS_LIST_FILE}" ")\n")
include("${SCRIPTS_LIST_FILE}")

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