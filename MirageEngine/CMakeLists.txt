# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES CONFIGURE_DEPENDS 
	"application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Toolbox/Source/*.cpp"

)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS CONFIGURE_DEPENDS 
	"application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include/*.inl"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Toolbox/Include/*.h"
    "${CMAKE_SOURCE_DIR}/Scripts/Source/*.h"
)
# Project executable
add_library(MirageEngine SHARED    
    ${MIRAGE_ENGINE_SOURCES}
    ${MIRAGE_ENGINE_HEADERS})

# Include directories
target_include_directories(MirageEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include/RHI"
)

target_include_directories(MirageEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Src"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Src/RHI"
)

target_precompile_headers(MirageEngine PUBLIC "Source/Include/pch.h")

target_compile_options(MirageEngine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Zi>        # Windows
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>  # Linux/Mac
)

# Link third-party libraries
target_link_libraries(MirageEngine PUBLIC third_party)

target_compile_definitions(MirageEngine PRIVATE ENGINE_EXPORTS)
# Ensure import library is always generated
set_target_properties(MirageEngine PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# Copy all asset in the build
set(ASSETS_SOURCE "${CMAKE_SOURCE_DIR}/Assets")
set(ASSETS_DEST "${MIRAGE_BIN_OUTPUT_DIR}/Assets")

if(EXISTS ${ASSETS_SOURCE})
    add_custom_command(TARGET MirageEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_DEST}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSETS_SOURCE}"
            "${ASSETS_DEST}"
        COMMENT "Copying assets to ${ASSETS_DEST}"
        VERBATIM
    )
    message(STATUS "Assets will be copie from ${ASSETS_SOURCE}")
else()
    message(STATUS "No Assets folder found at ${ASSETS_SOURCE} skip asset copy")
endif()

add_custom_command(TARGET MirageEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:MirageEngine>
        $<TARGET_FILE_DIR:MirageEngineEditor>
)


set(ENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include")
set(TOOLBOX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Tools/ToolBox/Source/Include")

add_custom_command(TARGET MirageEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ENGINE_INCLUDE_DIR}"
        "${MIRAGE_BIN_OUTPUT_DIR}/Include/Engine"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TOOLBOX_INCLUDE_DIR}"
        "${MIRAGE_BIN_OUTPUT_DIR}/Include/ToolBox"
)