# Adds an executable target with the given name and sources.
# Can be used to add executables to the project.
# Can be executed with: cmake --build . --target run_<NAME>

find_package(EDM4HEP 0.99 REQUIRED)
find_package(podio 1.0 REQUIRED)
find_package(DD4hep REQUIRED)

function(add_exec NAME)
  add_executable("${NAME}" "executables/${NAME}.cxx" ${ARGN})

  target_include_directories("${NAME}" PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  target_link_libraries("${NAME}" PRIVATE
    FastCaloSim::FastCaloSim
    FastCaloSim::Param
    DD4hep::DDCore # For bitfield decoder
    EDM4HEP::edm4hep # For dd4hep reading
    podio::podio # For dd4hep reading
    podio::podioRootIO # For dd4hep reading
  )

  target_compile_features("${NAME}" PRIVATE cxx_std_17)

  add_custom_target("run_${NAME}"
    COMMAND "${NAME}"
    VERBATIM
  )

  add_dependencies("run_${NAME}" "${NAME}")

  deactivate_checks(${NAME})
endfunction()
