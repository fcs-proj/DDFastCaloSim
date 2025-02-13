# Define minimum CMake version
cmake_minimum_required(VERSION 3.30)
# Define test project
project(DDFastCaloSimTests LANGUAGES CXX)

# ---- Test Base Directory ----
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DTEST_BASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DTEST_OUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/output/")
add_definitions(-DTEST_REFS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/refs/")
add_definitions(-DTEST_CONFIGS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/configs/")
add_definitions(-DPYTHON_SCRIPTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/python/scripts/")

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


add_executable(ODD_TransportTests source/ODDTransportTests.cxx)
target_link_libraries(ODD_TransportTests PRIVATE gtest_main nlohmann_json::nlohmann_json)
target_include_directories(ODD_TransportTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add simulation test with modified gun settings
add_test(
    NAME ODD_Simulation
    COMMAND ODD_TransportTests
)
set_tests_properties(
  ODD_Simulation
  PROPERTIES
    ENVIRONMENT
      "LD_PRELOAD=${DDFastCaloSim_LIB};PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}:$ENV{PYTHONPATH};TEST_DIR=${TEST_DIR}"
)
