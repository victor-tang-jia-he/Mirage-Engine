# Project executable
add_executable(MirageEngineEditor)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE_EDITOR_SOURCE
	"*.cpp"
	"*.h"
	"*.hpp"
)

target_sources(MirageEngineEditor PRIVATE ${MIRAGE_ENGINE_EDITOR_SOURCE} "Include/Inspector.h" "Source/Inspector.cpp" "Include/Game.h" "Source/Game.cpp" "Include/Hierarchy.h" "Source/Hierarchy.cpp")

# Include directories
target_include_directories(MirageEngineEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link third-party libraries
target_link_libraries(MirageEngineEditor PRIVATE ThirdParty)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngineEditor PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngineEditor PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()