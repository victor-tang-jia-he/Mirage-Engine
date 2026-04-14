if(NOT TOOLBOX_BUILD_TESTS)
    return()
endif()

add_executable(toolbox_tests)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")
target_sources(toolbox_tests PRIVATE ${TEST_SOURCES})

target_precompile_headers(toolbox_tests PRIVATE pch.h)

target_link_libraries(toolbox_tests
    PRIVATE
        Toolbox
        GTest::gtest_main
)

target_compile_features(toolbox_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(toolbox_tests)