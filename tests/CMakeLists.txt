add_executable(unit_tests
    main.cpp
    test_task1.cpp
    test_task2.cpp
    test_task3.cpp
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    task1_lib
    task2_lib
    task3_lib
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
