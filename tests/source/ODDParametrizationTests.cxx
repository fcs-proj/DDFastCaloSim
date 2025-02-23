#include <cstdlib>
#include <filesystem>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <gtest/gtest.h>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "TestHelpers/IOManager.h"
#include "TestHelpers/LogComparer.h"

TEST(ODDParametrizationTests, ODDParametrization)
{
  // Create the output directory for the test
  const std::string output_dir =
      TestHelpers::IOManager::create_test_output_dir();
  // Get the path to the reference files
  const std::string ref_dir = TestHelpers::IOManager::ref_dir();

  // Set test directory as environment variable
  setenv("TEST_WORKING_DIR", output_dir.c_str(), 1);

  /// -------------------------------------------------------------------
  /// Pre-load the DDFastCaloSim library
  /// -------------------------------------------------------------------

  /// TODO: instead of this we should be able to just source
  /// thisDDFastCaloSim.sh however something in key4hep is not working properly
  TestHelpers::IOManager::preload_ddfastcalosim();

  /// -------------------------------------------------------------------
  /// Execute the Geant4 simulation
  /// -------------------------------------------------------------------

  // Change the current directory to the output directory
  std::filesystem::current_path(output_dir);

  // Steering file with test configuration
  const std::string steering_file =
      std::string(TEST_CONFIGS_DIR) + "/OpenDataDetector/parametrization.py";

  // Name of ROOT file with Geant4 simulation output
  const std::string sim_output_file = "out.root";

  // Name of the log file
  const std::string log_file = "out.log";

  // Build the simulation command
  const std::string cmd = "ddsim --steeringFile " + steering_file
      + " --outputFile " + sim_output_file + " 2>&1 | tee " + log_file;

  // Execute the command
  int status = system(cmd.c_str());
  ASSERT_EQ(status, 0) << "Command failed: " << cmd;

  /// -------------------------------------------------------------------
  /// Compare the log file with the reference
  /// -------------------------------------------------------------------

  // Reference log file
  const std::string ref_log_file = ref_dir + log_file;

  LogComparer comparer(ref_log_file, log_file);
  // Lines containing these patterns will be ignored in the comparison
  comparer.setIgnorePatterns({"DDSim.Helper.Filter INFO ReqFilt",
                              "XMLLoader        INFO  +++ Processing XML file:",
                              "HEPMC3FileReader INFO  Created file reader"});

  ASSERT_TRUE(comparer.compareLogs())
      << "Log files differ! See the filtered differences above.";

  /// -------------------------------------------------------------------
  /// Check parametrization output file for consistency
  /// -------------------------------------------------------------------

  // Name of ROOT file with parametrization output
  const std::string param_output_file = "param_output.root";

  ASSERT_TRUE(std::filesystem::exists(param_output_file))
      << "File " << param_output_file << " does not exist!";

  // Open the ROOT file
  TFile file(param_output_file.c_str(), "READ");
  ASSERT_TRUE(file.IsOpen())
      << "Failed to open ROOT file: " << param_output_file;

  // Get the TTree
  auto tree = file.Get<TTree>("param_info");
  ASSERT_TRUE(tree) << "TTree 'param_info' not found in ROOT file.";

  // Check number of entries (should be exactly 5)
  const int n_entries = tree->GetEntries();
  const int n_expected_entries = 5;
  ASSERT_EQ(n_entries, n_expected_entries)
      << "Expected " << n_expected_entries << " events, got " << n_entries;

  // Set up branch reading
  std::vector<TFCSExtrapolationState>* extrapolations = nullptr;
  tree->SetBranchAddress("extrapolations", &extrapolations);

  for (int i = 0; i < n_entries; ++i) {
    tree->GetEntry(i);
    // Check that there is exactly one std::vector<TFCSExtrapolationStates> in
    // each event
    ASSERT_TRUE(extrapolations) << "Branch 'extrapolations' not found " << i;
    ASSERT_EQ(extrapolations->size(), 1)
        << "Event " << i
        << " does not contain exactly 1 TFCSExtrapolationStates element.";
  }

  // Close the file
  file.Close();
}
