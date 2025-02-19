# ----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.30)

include(cmake/prelude.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(DDFastCaloSim)

include(cmake/project-is-top-level.cmake)
include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

message(STATUS "Configuring project ${PROJECT_NAME}")

# ----------------------------------------------------------------------------
# Configure C++ Standard
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to use")

# Ensure the standard is strictly required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------------------------------------------------
# Find required packages
find_package(Geant4 REQUIRED)
find_package(DD4hep REQUIRED COMPONENTS DDG4 DDParsers DDCore DDDetectors)

# ----------------------------FastCaloSim-------------------------------------
include(FetchContent)
# LWTNN is not shipped by DD4hep but a dependency of FastCaloSim
# We'll give FastCaloSim a hint where to find it
set(lwtnn_ROOT "/cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt"
  CACHE PATH "Where to find lwtnn" FORCE
)

FetchContent_Declare(
  FastCaloSim
  GIT_REPOSITORY https://github.com/fcs-proj/FastCaloSim.git
  GIT_TAG v0.9.0-alpha
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(FastCaloSim)

# Enable RPATH and set compiler flags for DD4hep
set(DD4HEP_SET_RPATH ON)
dd4hep_set_compiler_flags()
dd4hep_use_python_executable()

# ----------------------------------------------------------------------------
# Find sources
file(GLOB DDFastCaloSim_SOURCES source/*.cxx)
# Add test sources
add_subdirectory(tests)

# ----------------------------------------------------------------------------
# Create the plugin library
add_dd4hep_plugin(${PROJECT_NAME} SHARED ${DDFastCaloSim_SOURCES})

# Set the include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests)

# Link plugins against Geant4 and DD4hep
target_link_libraries(${PROJECT_NAME} PUBLIC ${Geant4_LIBRARIES} DD4hep::DDCore DD4hep::DDG4 FastCaloSim::FastCaloSim nlohmann_json::nlohmann_json)

# ----------------------------------------------------------------------------
# Installation configuration for the plugin library
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
)

# DD4hep package instantiation
dd4hep_instantiate_package(${PROJECT_NAME})

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
