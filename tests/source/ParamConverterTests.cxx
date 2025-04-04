#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include <TBranch.h>
#include <TFile.h>
#include <TInterpreter.h>
#include <TSystem.h>
#include <TTree.h>
#include <gtest/gtest.h>

#include "TestConfig/ParamConverterTestsConfig.h"
#include "TestHelpers/IOManager.h"
#include "TestHelpers/LogComparer.h"

TEST(ParamConverterTests, ParamConverterTest)
{
  // Let ROOT load the Param dictionary
  const char* ddFCSLib = std::getenv("FastCaloSimParam_LIB");

  ASSERT_NE(ddFCSLib, nullptr)
      << "Environment variable FastCaloSimParamLib is not set!";

  // Load the library with ROOT's gSystem
  int loadStatus = gSystem->Load(ddFCSLib);
  if (loadStatus != 0) {
    std::cerr << "gSystem->Load() failed: " << gSystem->GetErrorStr()
              << std::endl;
  }

  // Create the output directory for the test
  const std::string test_output_dir =
      TestHelpers::IOManager::create_test_output_dir();
  // Reference directory for the test
  const std::string test_ref_dir = TestHelpers::IOManager::ref_dir();

  // Build command
  const std::string cmd =
      ParamConverterTestsConfig::converter_cmd(test_output_dir);
  std::cout << "Executing command: " << cmd << std::endl;

  int status = system(cmd.c_str());
  ASSERT_EQ(status, 0) << "Executable failed: " << cmd;

  const std::string output_file =
      ParamConverterTestsConfig::converter_output_path(test_output_dir);

  // Check that the output file was created
  ASSERT_TRUE(std::filesystem::exists(output_file))
      << "Output file " << output_file << " was not generated.";

  // Open ROOT output file
  TFile fout(output_file.c_str(), "READ");
  ASSERT_TRUE(fout.IsOpen()) << "Failed to open output ROOT file.";

  // Verify TTree structure
  TTree* tree = dynamic_cast<TTree*>(fout.Get("FCS_ParametrizationInput"));
  ASSERT_TRUE(tree)
      << "TTree 'FCS_ParametrizationInput' not found in output ROOT file.";

  // Check the total number of entries in the tree
  const int n_entries = tree->GetEntries();

  // Ensure the number of entries the tree is as expected
  ASSERT_EQ(n_entries, 5) << "Expected 5 entries in in TTree, got "
                          << n_entries;

  // Verify presence of required branch
  TBranch* branch = tree->GetBranch("newTTC_IDCaloBoundary_eta");
  ASSERT_TRUE(branch)
      << "Branch 'newTTC_IDCaloBoundary_eta' not found in TTree.";

  // Compare the log file with the reference
  const std::string ref_log_file =
      test_ref_dir + ParamConverterTestsConfig::log_file_name;
  const std::string test_log_file =
      test_output_dir + ParamConverterTestsConfig::log_file_name;

  LogComparer comparer(ref_log_file, test_log_file);

  comparer.setIgnorePatterns({"[createParamInput] Output file:"});

  ASSERT_TRUE(comparer.compareLogs()) << "Log files differ!";

  fout.Close();
}
