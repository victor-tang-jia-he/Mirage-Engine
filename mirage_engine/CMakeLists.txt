# Project executable
add_library(MirageEngine STATIC)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES
	"RHI/Src/*.cpp"
	"RHI/Src/*.c"
	"Components/*.c"
	"Components/*.cpp"
	"application.cpp"
	"main.cpp"
)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS
	"application.h"
	"Components/*.h"
	"Components/*.hpp"
	"RHI/Include/*.hpp"
	"RHI/Include/*.h"
)

target_sources(MirageEngine PRIVATE ${MIRAGE_ENGINE_SOURCES} ${MIRAGE_ENGINE_HEADERS})

# Include directories
target_include_directories(MirageEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/RHI/Include"
)

target_include_directories(MirageEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/RHI/Src"
    ${CMAKE_CURRENT_SOURCE_DIR}/Components
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