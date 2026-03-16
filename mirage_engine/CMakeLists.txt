# Project executable
add_executable(MirageEngine)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_SOURCES
	"RHI/Src/*.cpp"
	"RHI/Src/*.c"
)

file(GLOB_RECURSE MIRAGE_ENGINE_HEADERS
	"RHI/Include/*.hpp"
	"RHI/Include/*.h"
)

target_sources(MirageEngine PRIVATE ${MIRAGE_ENGINE_SOURCES} ${MIRAGE_ENGINE_HEADERS})

# Include directories
target_include_directories(MirageEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/RHI/Include
)

# Link third-party libraries
target_link_libraries(MirageEngine PRIVATE third_party)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngine PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()