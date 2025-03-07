#include <cstdlib>
#include <filesystem>
#include <string>

#include "TransportTests.h"

#include <gtest/gtest.h>

#include "TestConfig/TransportTestsConfig.h"
#include "TestHelpers/IOManager.h"
#include "TestHelpers/JsonComparer.h"
#include "TestHelpers/LogComparer.h"

TEST_F(TransportTests, Transport)
{
  // Create the output directory for the test
  const std::string test_output_dir =
      TestHelpers::IOManager::create_test_output_dir();
  const std::string test_ref_dir = TestHelpers::IOManager::ref_dir();

  // Set test directory as environment variable
  setenv("TEST_WORKING_DIR", test_output_dir.c_str(), 1);

  // Change the current directory to the output directory
  std::filesystem::current_path(test_output_dir);

  /// -------------------------------------------------------------------
  /// Execute trabnsport
  /// -------------------------------------------------------------------

  // Execute the simulation command
  int status = system(TransportTestsConfig::sim_cmd(test_output_dir).c_str());
  ASSERT_EQ(status, 0) << "Simulation command failed.";

  // Execute transport plotting
  status =
      system(TransportTestsConfig::plot_transport_cmd(test_output_dir).c_str());
  ASSERT_EQ(status, 0) << "Plot transport command failed.";

  // Compare log files
  const std::string ref_log_file =
      test_ref_dir + TransportTestsConfig::log_file_name;
  LogComparer comparer(ref_log_file,
                       test_output_dir + TransportTestsConfig::log_file_name);
  comparer.setIgnorePatterns({"DDSim.Helper.Filter INFO ReqFilt",
                              "XMLLoader        INFO  +++ Processing XML file:",
                              "HEPMC3FileReader INFO  Created file reader",
                              "Geant4Output2EDM4hep INFO  Opened"});

  ASSERT_TRUE(comparer.compareLogs()) << "Log files differ!";

  // Compare transport results with references
  const std::string ref_transport_tracks =
      test_ref_dir + "transport_tracks.json";
  JsonComparer json_tool(1e-3);
  ASSERT_TRUE(json_tool.compare(
      test_output_dir + TransportTestsConfig::transport_tracks_json,
      ref_transport_tracks));
}
