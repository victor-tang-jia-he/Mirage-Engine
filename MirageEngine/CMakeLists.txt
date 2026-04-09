# Project executable
add_library(MirageEngine STATIC)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES
	"Source/Src/*.cpp"
	"Source/Src/RHI/*.cpp"
	"Components/*.c"
	"Components/*.cpp"
	"application.cpp"
	"main.cpp"
)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS
	"Source/Src/*.h"
	"Source/Src/RHI/*.h"
	"application.h"
	"Components/*.h"
	"Components/*.hpp"
)

target_sources(MirageEngine PRIVATE ${MIRAGE_ENGINE_SOURCES} ${MIRAGE_ENGINE_HEADERS} "Source/Src/SceneGraph/scene_graph.cpp")

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
if(EXISTS ${PROJECT_SOURCE_DIR}/MirageEngine/Assets)
	add_custom_command(TARGET MirageEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_SOURCE_DIR}/MirageEngine/Assets"
			"${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>/Assets")
	message(STATUS "Assets copied successfully")
endif()


# Debug
if(WIN32)
  add_custom_command(
    TARGET MirageEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:cpptrace::cpptrace>
    "${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
endif()