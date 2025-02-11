# Define minimum CMake version
cmake_minimum_required(VERSION 3.30)
# Define test project
project(DDFastCaloSimTests LANGUAGES CXX)

# ---- Test Base Directory ----
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ---- Create test output Location ----
file(MAKE_DIRECTORY ${TEST_DIR}/output)

# ---- Dependencies ----

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ---- Tests ----

include(GoogleTest)

# Set the DDFastCaloSim library path
set(DDFastCaloSim_LIB "${CMAKE_SOURCE_DIR}/build/libDDFastCaloSim.so")
message(STATUS "Using DDFastCaloSim_LIB = ${DDFastCaloSim_LIB}")

# Add simulation test with modified gun settings
add_test(
    NAME ODD_Simulation
    COMMAND ddsim --steeringFile ${CMAKE_CURRENT_SOURCE_DIR}/configs/OpenDataDetector/simulation.py
)
set_tests_properties(
  ODD_Simulation
  PROPERTIES
    ENVIRONMENT
      "LD_PRELOAD=${DDFastCaloSim_LIB};PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}:$ENV{PYTHONPATH};TEST_DIR=${TEST_DIR}"
)
