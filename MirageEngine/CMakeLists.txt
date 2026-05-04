
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

target_precompile_headers(MirageEngine PRIVATE "Source/Include/pch.h")

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
add_custom_command(TARGET MirageEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:MirageEngine>/Assets"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:MirageEngine>/Assets"
)