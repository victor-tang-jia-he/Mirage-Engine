# Project executable
add_executable(MirageEngineEditor)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES
	"Source/*.cpp"
	"Source/*.c"
)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS
	"Include/*.hpp"
	"Include/*.h"
)

target_sources(MirageEngineEditor PRIVATE ${MIRAGE_ENGINE_SOURCES} ${MIRAGE_ENGINE_HEADERS})

# Include directories
target_include_directories(MirageEngineEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

# Link third-party libraries
target_link_libraries(MirageEngineEditor PRIVATE third_party)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngineEditor PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngineEditor PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()