# Project executable
add_library(MirageEngine STATIC)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES
	"application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Toolbox/Source/*.cpp"

)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS
	"application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/Toolbox/Include/*.h"
)

target_sources(MirageEngine PRIVATE ${MIRAGE_ENGINE_SOURCES} ${MIRAGE_ENGINE_HEADERS})

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

target_compile_options(MirageEngine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Zi>        # Windows
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>  # Linux/Mac
)

# Link third-party libraries
target_link_libraries(MirageEngine PRIVATE third_party)

# Copy all asset in the build
set(ASSETS_SOURCE "${CMAKE_SOURCE_DIR}/MirageEngine/Assets")
set(ASSETS_DEST "${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>/Assets")

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