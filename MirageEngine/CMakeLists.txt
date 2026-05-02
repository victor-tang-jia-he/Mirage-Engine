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
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Include/*.inl"
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


set(ASSETS_MIRAGE_ENGINE "${CMAKE_SOURCE_DIR}/MirageEngine/Assets")
set(ASSETS_MIRAGE_EDITOR "${CMAKE_SOURCE_DIR}/MirageEngineEditor/Assets")
set(ASSETS_MIRAGE_BUILD "${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_${CMAKE_BUILD_TYPE}/Assets")
set(ASSETS_MAIN "${CMAKE_SOURCE_DIR}/Assets")

if(EXISTS ${ASSETS_MAIN})
    file(MAKE_DIRECTORY "${ASSETS_MIRAGE_BUILD}")
    file(COPY "${ASSETS_MAIN}/" DESTINATION "${ASSETS_MIRAGE_BUILD}")
    file(COPY "${ASSETS_MAIN}/" DESTINATION "${ASSETS_MIRAGE_ENGINE}")
    file(COPY "${ASSETS_MAIN}/" DESTINATION "${ASSETS_MIRAGE_EDITOR}")
    message(STATUS "Assets copied from ${ASSETS_MAIN}")
else()
    message(STATUS "No Assets folder found at ${ASSETS_MAIN}")
endif()