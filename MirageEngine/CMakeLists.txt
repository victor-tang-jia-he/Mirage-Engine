# Project executable
add_executable(MirageEngine "main.cpp")

# Sources
# file(GLOB_RECURSE MIRAGE_ENGINE_SOURCE
# 	"*.cpp"
# 	"*.h"
# 	"*.hpp"
# )
# 
# target_sources(MirageEngine PRIVATE ${MIRAGE_ENGINE_SOURCE})

# Include directories
target_include_directories(MirageEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link third-party libraries
target_link_libraries(MirageEngine PRIVATE ThirdParty)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngine PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()