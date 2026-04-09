add_executable(toolbox_tests)

file(GLOB_RECURSE TOOLBOX_SOURCES CONFIGURE_DEPENDS
	"*.cpp"
)

target_sources(toolbox_tests PRIVATE ${TOOLBOX_SOURCES})

target_link_libraries(toolbox_tests
    PRIVATE
        Toolbox
        GTest::gtest_main
        glm::glm
)

target_compile_features(toolbox_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(toolbox_tests)