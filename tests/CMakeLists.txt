# Define minimum CMake version
cmake_minimum_required(VERSION 3.30)

# Define test project
project(DDFastCaloSimTests LANGUAGES CXX)

# Builds tests with valgrind memory profiling enabled
# Run with cmake -DENABLE_VALGRIND_TESTS=ON to enable
option(ENABLE_VALGRIND_TESTS "Enable Valgrind memory profiling test variants" OFF)

if(ENABLE_VALGRIND_TESTS)
  find_program(VALGRIND_EXECUTABLE valgrind)

  if(NOT VALGRIND_EXECUTABLE)
    message(FATAL_ERROR
      "Valgrind must be installed or on your PATH when ENABLE_VALGRIND_TESTS=ON")
  endif()

  message(STATUS "Running tests under Valgrind (--tool=massif). Output files will be saved as *.massif and can be viewed with 'ms_print <file>.massif'")
endif()

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

# ---- FastCaloSim Libraries ----
set(FastCaloSim_LIB "${CMAKE_BINARY_DIR}/_deps/fastcalosim-build/libFastCaloSim.so")
set(FastCaloSimParam_LIB "${CMAKE_BINARY_DIR}/_deps/fastcalosim-build/param/libFastCaloSim_Param.so")

# ---- DDFastCaloSim Library ----
set(DDFastCaloSim_LIB "${CMAKE_BINARY_DIR}/libDDFastCaloSim.so")

message(STATUS "Using FastCaloSim_LIB = ${FastCaloSim_LIB}")
message(STATUS "Using FastCaloSimParam_LIB = ${FastCaloSimParam_LIB}")
message(STATUS "Using DDFastCaloSim_LIB = ${DDFastCaloSim_LIB}")

# ---- Tests ----
include(GoogleTest)

# ---- Add tests ----
include(../cmake/tests.cmake)
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*Tests.cxx")

add_tests("${TEST_SOURCES}")
