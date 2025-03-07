#pragma once

#include <string>

#include <gtest/gtest.h>

#include "TestHelpers/IOManager.h"

class ParamTestsConfig : public ::testing::Environment
{
public:
  inline static const std::string steering_file =
      std::string(TEST_CONFIGS_DIR) + "OpenDataDetector/parametrization.py";

  /// @brief Note: param_output is currently hardcoded in the writer
  inline static const std::string param_output_name = "param_output.root";
  inline static const std::string dd4hep_output_name = "dd4hep_out.root";
  inline static const std::string log_file_name = "sim.log";

  static auto sim_cmd(const std::string& test_output_dir) -> std::string
  {
    const std::string dd4hep_output_path = test_output_dir + dd4hep_output_name;
    const std::string log_output_path = test_output_dir + log_file_name;

    const std::string cmd = "ddsim --steeringFile " + steering_file
        + " --outputFile " + dd4hep_output_path + " 2>&1 | tee "
        + log_output_path;

    return cmd;
  }
};
