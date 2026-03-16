# Project executable
add_executable(MirageEngine)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES
	"source/src/*.cpp"
	"source/src/*.c"
)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS
	"source/include/*.hpp"
	"source/include/*.h"
)

target_sources(MirageEngine PRIVATE ${MIRAGE_ENGINE_SOURCES} ${MIRAGE_ENGINE_HEADERS})

# Include directories
target_include_directories(MirageEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include
)

# Link third-party libraries
target_link_libraries(MirageEngine PRIVATE third_party)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngine PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Copy all asset in the build
if(EXISTS ${PROJECT_SOURCE_DIR}/mirage_engine/assets)
	add_custom_command(TARGET MirageEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_SOURCE_DIR}/mirage_engine/assets"
			"$<TARGET_FILE_DIR:MirageEngine>/assets")
endif()