# Ex¨¦cumle de test
add_executable(toolbox_tests
    Test.cpp
)

# Lier les biblioth¨¨ques n¨¦cessaires
target_link_libraries(toolbox_tests
    PRIVATE
        Toolbox
        GTest::gtest_main
        GTest::gmock_main
        glm::glm
)

# Inclure les r¨¦pertoires n¨¦cessaires
target_include_directories(toolbox_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Sp¨¦cifier les features C++
target_compile_features(toolbox_tests PRIVATE cxx_std_17)

# Activer les tests
include(GoogleTest)
gtest_discover_tests(toolbox_tests)