#pragma once

#include <string>

#include <gtest/gtest.h>

#include "TestHelpers/IOManager.h"

class TransportTestsConfig : public ::testing::Environment
{
public:
  inline static const std::string steering_file =
      std::string(TEST_CONFIGS_DIR) + "/OpenDataDetector/transport.py";

  inline static const std::string output_file_name = "dd4hep_out.root";
  inline static const std::string log_file_name = "dd4hep_out.log";
  inline static const std::string transport_tracks_json =
      "transport_tracks.json";
  inline static const std::string transport_tracks_png = "transport_tracks.png";

  static auto sim_cmd(const std::string& test_output_dir) -> std::string
  {
    const std::string output_path = test_output_dir + output_file_name;
    const std::string log_path = test_output_dir + log_file_name;

    return "ddsim --steeringFile " + steering_file + " --outputFile "
        + output_path + " 2>&1 | tee " + log_path;
  }

  static auto plot_transport_cmd(const std::string& test_output_dir)
      -> std::string
  {
    const std::string input_json = test_output_dir + transport_tracks_json;
    const std::string output_png = test_output_dir + transport_tracks_png;
    const std::string composition_config =
        std::string(PYTHON_INPUT_DIR) + "composition_config.json";

    return "python3 " + std::string(PYTHON_SCRIPTS_DIR) + "plot_transport.py"
        + " --input " + input_json + " --output " + output_png + " --label \"\""
        + " --track_zoom --region_composition " + composition_config;
  }
};
