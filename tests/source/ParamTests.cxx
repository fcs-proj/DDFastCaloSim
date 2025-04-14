#include <cstdlib>
#include <filesystem>
#include <string>

#include "ParamTests.h"

#include <TFile.h>
#include <TTree.h>
#include <gtest/gtest.h>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "TestHelpers/LogComparer.h"

TEST_F(ParamTests, Parametrization)
{
  const std::string test_output_dir =
      TestHelpers::IOManager::create_test_output_dir();

  const std::string test_ref_dir = TestHelpers::IOManager::ref_dir();

  setenv("TEST_WORKING_DIR", test_output_dir.c_str(), 1);

  // Change the current directory to the output directory
  // This is to avoid having to pass absolute path for param_output_file
  std::filesystem::current_path(test_output_dir);

  /// -------------------------------------------------------------------
  /// Execute the Geant4 simulation
  /// -------------------------------------------------------------------

  // Execute the Geant4 simulation
  const std::string cmd = ParamTestsConfig::sim_cmd(test_output_dir);

  std::cout << "Executing command: " << cmd << std::endl;

  int status = system(cmd.c_str());
  ASSERT_EQ(status, 0) << "Command failed: " << cmd;

  // Compare the log file with the reference
  const std::string ref_log_file =
      test_ref_dir + ParamTestsConfig::log_file_name;
  const std::string test_log_file =
      test_output_dir + ParamTestsConfig::log_file_name;

  LogComparer comparer(ref_log_file, test_log_file);

  comparer.setIgnorePatterns({"DDSim.Helper.Filter INFO ReqFilt",
                              "XMLLoader        INFO  +++ Processing XML file:",
                              "HEPMC3FileReader INFO  Created file reader",
                              "Geant4Output2EDM4hep INFO  Opened"
                              "G4GDML: Reading"});

  ASSERT_TRUE(comparer.compareLogs()) << "Log files differ!";

  // Check parametrization output file
  ASSERT_TRUE(std::filesystem::exists(ParamTestsConfig::param_output_name))
      << "File " << ParamTestsConfig::param_output_name << " does not exist!";

  TFile file(ParamTestsConfig::param_output_name.c_str(), "READ");
  ASSERT_TRUE(file.IsOpen())
      << "Failed to open ROOT file: " << ParamTestsConfig::param_output_name;

  auto tree = file.Get<TTree>("param_info");
  ASSERT_TRUE(tree) << "TTree 'param_info' not found in ROOT file.";

  const int n_entries = tree->GetEntries();
  const int n_expected_entries = 5;
  ASSERT_EQ(n_entries, n_expected_entries)
      << "Expected " << n_expected_entries << " events, got " << n_entries;

  std::vector<TFCSExtrapolationState>* extrapolations = nullptr;
  tree->SetBranchAddress("extrapolations", &extrapolations);

  for (int i = 0; i < n_entries; ++i) {
    tree->GetEntry(i);
    ASSERT_TRUE(extrapolations)
        << "Branch 'extrapolations' not found in event " << i;
    ASSERT_EQ(extrapolations->size(), 1)
        << "Event " << i
        << " does not contain exactly 1 TFCSExtrapolationStates element.";
  }

  file.Close();
}
