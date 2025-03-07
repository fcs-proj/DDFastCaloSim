# Define minimum CMake version
cmake_minimum_required(VERSION 3.30)

# Define test project
project(DDFastCaloSimTests LANGUAGES CXX)

# ---- Test Base Directory ----

# Python environment variables passed in cmake/tests.cmake
set(TEST_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Definitions for google tests
add_definitions(-DTEST_BASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DTEST_OUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/output/")
add_definitions(-DTEST_REFS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/refs/")
add_definitions(-DTEST_CONFIGS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/configs/")
add_definitions(-DPYTHON_SCRIPTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/python/scripts/")
add_definitions(-DPYTHON_INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/python/input/")
add_definitions(-DCONVERTER_EXECUTABLE="${CMAKE_BINARY_DIR}/converter/createParamInput")

# ---- Create test output Location ----
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

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

# Set the DDFastCaloSim library
set(DDFastCaloSim_LIB "${CMAKE_BINARY_DIR}/libDDFastCaloSim.so")

message(STATUS "Using DDFastCaloSim_LIB = ${DDFastCaloSim_LIB}")

# ---- Tests ----
include(GoogleTest)

# ---- Add tests ----
include(../cmake/tests.cmake)
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*Tests.cxx")

add_tests("${TEST_SOURCES}")
