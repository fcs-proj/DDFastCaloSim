function(add_tests TEST_SOURCES)
    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        add_executable(${TEST_NAME} ${TEST_FILE})

        target_link_libraries(${TEST_NAME} PRIVATE
            gtest
            gtest_main
            nlohmann_json::nlohmann_json
            ROOT::Core
            FastCaloSim::FastCaloSim)

        target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR})

        # Build the env var list once
        set(ENV_VARS
            "TEST_BASE_DIR=${TEST_BASE_DIR}"
            "FastCaloSim_LIB=${FastCaloSim_LIB}"
            "FastCaloSimParam_LIB=${FastCaloSimParam_LIB}"
            "DDFastCaloSim_LIB=${DDFastCaloSim_LIB}"
            "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}:$ENV{LD_LIBRARY_PATH}"
            "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}:$ENV{PYTHONPATH}"
        )

        if(ENABLE_VALGRIND_TESTS)
            # Format env for inline shell command
            string(REPLACE ";" " " ENV_STRING "${ENV_VARS}")
            string(APPEND ENV_STRING " LD_PRELOAD=${DDFastCaloSim_LIB}")

            # Use a single-line shell with env and valgrind
            add_test(NAME ${TEST_NAME}
                COMMAND /bin/bash -c
                "${ENV_STRING} ${VALGRIND_EXECUTABLE} --tool=massif --pages-as-heap=yes --massif-out-file=${TEST_NAME}.massif $<TARGET_FILE:${TEST_NAME}>"
            )
        else()
            add_test(NAME ${TEST_NAME}
                COMMAND $<TARGET_FILE:${TEST_NAME}>)
            set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "${ENV_VARS}")
        endif()
    endforeach()
endfunction()
