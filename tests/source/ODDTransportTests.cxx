#include <cstdlib>
#include <filesystem>
#include <string>

#include <gtest/gtest.h>

#include "TestHelpers/IOManager.h"
#include "TestHelpers/JsonComparer.h"
#include "TestHelpers/LogComparer.h"

TEST(ODDTransportTests, ODDTransport)
{
  // Create the output directory for the test
  const std::string output_dir =
      TestHelpers::IOManager::create_test_output_dir();
  // Get the path to the reference files
  const std::string ref_dir = TestHelpers::IOManager::ref_dir();

  // Change the current directory to the output directory
  std::filesystem::current_path(output_dir);

  // Steering file with test configuration
  const std::string steering_file =
      std::string(TEST_CONFIGS_DIR) + "/OpenDataDetector/simulation.py";

  // Name of ROOT file with simulation output
  const std::string output_file = "out.root";

  // Name of the log file
  const std::string log_file = "out.log";

  // Build the simulation command
  const std::string cmd = "ddsim --steeringFile " + steering_file
      + " --outputFile " + output_file + " 2>&1 | tee " + log_file;

  // Execute the simulation command
  int status = system(cmd.c_str());
  ASSERT_EQ(status, 0) << "Simulation command failed: " << cmd;

  // Plot the transport tracks
  std::string label = "ODD Transport";
  std::string plot_transport_exec = "python3 " + std::string(PYTHON_SCRIPTS_DIR)
      + "plot_transport.py" + " --input transport_tracks.json"
      + " --output transport_tracks.png" + " --label \"" + label + "\""
      + " --track_zoom";

  ASSERT_TRUE(system(plot_transport_exec.c_str()) == 0);

  // Reference log file
  const std::string ref_log_file = ref_dir + "/" + log_file;

  LogComparer comparer(ref_log_file, log_file);
  // Set any prefixes to ignore in the log comparison
  comparer.setIgnoredPrefixes({
      "DDSim.Helper.Filter INFO ReqFilt"  // ordering of filters
      "XMLLoader        INFO  +++ Processing XML file:"  // file paths
  });
  ASSERT_TRUE(comparer.compareLogs())
      << "Log files differ! See the filtered differences above.";

  // Now compare transport results with references with a tolerance of 0.1
  // percent
  const std::string ref_transport_tracks = ref_dir + "/transport_tracks.json";

  const double tol = 1e-3;
  JsonComparer json_tool(tol);
  // Compare the transport data to references
  ASSERT_TRUE(json_tool.compare("transport_tracks.json", ref_transport_tracks));
}
