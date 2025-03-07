cmake_minimum_required(VERSION 3.14)

project(FastCaloSimParam CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/executable.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(FastCaloSim REQUIRED)
endif()

# Glob additional sources
file(GLOB SOURCES "source/*.cxx")

# Add executable
add_exec(createParamInput ${SOURCES})

add_folders(param)
