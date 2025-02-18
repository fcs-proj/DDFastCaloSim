function(add_tests TEST_SOURCES)
    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        # Add the test executable
        add_executable(${TEST_NAME} ${TEST_FILE})
        # Link the test executable with the GoogleTest and necessary libraries
        target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main nlohmann_json::nlohmann_json)
        # Add include directories
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        # Add the test to the test suite
        add_test(NAME ${TEST_NAME}
                COMMAND ${TEST_NAME})
        # Set the test properties: load the DDFastCaloSim library and set the PYTHONPATH
        set_tests_properties(${TEST_NAME} PROPERTIES
                                          ENVIRONMENT "LD_PRELOAD=${DDFastCaloSim_LIB};PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}:$ENV{PYTHONPATH};TEST_DIR=${TEST_DIR}")
    endforeach()
endfunction()
