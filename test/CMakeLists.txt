file(GLOB WORDLE_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cc")

add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)

foreach (wordle_test_source ${WORDLE_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(wordle_test_filename ${wordle_test_source} NAME)
    string(REPLACE ".cc" "" wordle_test_name ${wordle_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${wordle_test_name} EXCLUDE_FROM_ALL ${wordle_test_source})
    add_dependencies(check-tests ${wordle_test_name})
    add_dependencies(build-tests ${wordle_test_name})

    target_link_libraries(${wordle_test_name} wordle_shared rpc Catch2::Catch2WithMain)

    # Set test target properties and dependencies.
    set_target_properties(${wordle_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
        COMMAND ${wordle_test_name}
    )

    # Add the test under CTest.
    add_test(${wordle_test_name} ${CMAKE_BINARY_DIR}/test/${wordle_test_name})
endforeach(wordle_test_source ${wordle_TEST_SOURCES})