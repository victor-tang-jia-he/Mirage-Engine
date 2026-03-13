# Executable
add_executable(toolbox_tests
    Test.cpp
)

target_link_libraries(toolbox_tests
    PRIVATE
        Toolbox
        GTest::gtest_main
        GTest::gmock_main
        glm::glm
)

target_include_directories(toolbox_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_features(toolbox_tests PRIVATE cxx_std_17)

# Enable tests
include(GoogleTest)
gtest_discover_tests(toolbox_tests)