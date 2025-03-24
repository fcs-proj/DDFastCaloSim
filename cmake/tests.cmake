function(add_tests TEST_SOURCES)
    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        # Add the test executable
        add_executable(${TEST_NAME} ${TEST_FILE})

        # Link the test executable with the GoogleTest and necessary libraries
        target_link_libraries(${TEST_NAME} PRIVATE
            gtest
            gtest_main
            nlohmann_json::nlohmann_json
            ROOT::Core
            FastCaloSim::FastCaloSim)

        # Add include directories
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

        # Add the test to the test suite
        add_test(NAME ${TEST_NAME}
            COMMAND ${TEST_NAME})

        # Set the test properties
        # Propagate the dd4hep environment to tets
        # Preload the DDFastCaloSim library
        set(ENV_VARS
            "TEST_BASE_DIR=${TEST_BASE_DIR}"
            "FastCaloSim_LIB=${FastCaloSim_LIB}"
            "FastCaloSimParam_LIB=${FastCaloSimParam_LIB}"
            "DDFastCaloSim_LIB=${DDFastCaloSim_LIB}"
            "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}"
            "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}:$ENV{PYTHONPATH}"
        )

        set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "${ENV_VARS}")
    endforeach()
endfunction()
