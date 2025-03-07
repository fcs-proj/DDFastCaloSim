#pragma once

#include <string>

#include <gtest/gtest.h>

#include "TestHelpers/IOManager.h"

class ParamConverterTestsConfig : public ::testing::Environment
{
public:
  inline static const std::string param_name = "param_output.root";
  inline static const std::string dd4hep_name = "dd4hep_out.root";
  inline static const std::string converter_output_name =
      "FCS_ParametrizationInput.root";
  inline static const std::string log_file_name = "converter.log";

  // Take param + dd4hep output files from the reference directory
  inline static const std::string param_ref_dir =
      TestHelpers::IOManager::get_test_ref_dir("ParamTests", "Parametrization");

  inline static const std::string dd4hep_input_path =
      param_ref_dir + dd4hep_name;
  inline static const std::string param_input_path = param_ref_dir + param_name;

  static auto converter_output_path(const std::string& test_output_dir)
      -> std::string
  {
    return test_output_dir + converter_output_name;
  }

  static auto converter_cmd(const std::string& test_output_dir) -> std::string
  {
    const std::string log_output_path = test_output_dir + log_file_name;

    return std::string(CONVERTER_EXECUTABLE) + " " + param_input_path + " "
        + converter_output_path(test_output_dir) + " 2>&1 | tee "
        + log_output_path;
  }
};
