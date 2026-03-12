# Project executable
add_executable(MirageEngineEditor)

# Sources
file(GLOB_RECURSE MIRAGE_ENGINE__EDITOR_SOURCE 
    "*.cpp"
    "*.c"
)

target_sources(MirageEngineEditor PRIVATE ${MIRAGE_ENGINE__EDITOR_SOURCE})

# Include directories
target_include_directories(MirageEngineEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Projects/MirageEngine/include
)

# Link engine and third-party
target_link_libraries(MirageEngineEditor PRIVATE
    # MirageEngine  # Link the engine itself
    ThirdParty    # And third-party libs
)

# Set Warning level 4 and warnings as errors flags
if(MSVC)
	target_compile_options(MirageEngine PRIVATE /W4 /WX)
else()
	target_compile_options(MirageEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()