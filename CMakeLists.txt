#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.15)
project(DDFastCaloSim)

message(STATUS "Configuring project ${PROJECT_NAME}")

#----------------------------------------------------------------------------
# Configure C++ Standard
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensure the standard is strictly required
set(CMAKE_CXX_EXTENSIONS OFF)       # Disable compiler-specific extensions

#----------------------------------------------------------------------------
# Find required packages
find_package(Geant4 REQUIRED)
find_package(DD4hep REQUIRED COMPONENTS DDG4 DDParsers DDCore DDDetectors)

# Enable RPATH and set compiler flags for DD4hep
set(DD4HEP_SET_RPATH ON)
dd4hep_set_compiler_flags()
dd4hep_use_python_executable()

#----------------------------------------------------------------------------
# Source and header file configuration
file(GLOB sources source/*.cc)
set(headers ${PROJECT_SOURCE_DIR}/include/DDFastCaloSim/*.h)

#----------------------------------------------------------------------------
# Create the plugin library
add_dd4hep_plugin(${PROJECT_NAME} SHARED ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${Geant4_LIBRARIES} DD4hep::DDCore DD4hep::DDG4)

#----------------------------------------------------------------------------
# Installation configuration for the plugin library
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
)

# DD4hep package instantiation
dd4hep_instantiate_package(${PROJECT_NAME})

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

dd4hep_set_version(${PROJECT_NAME} MAJOR 1 MINOR 0 PATCH 0)
dd4hep_generate_rootmap(${PROJECT_NAME})

# Installation configuration for the detector library
install(TARGETS ${PROJECT_NAME})

