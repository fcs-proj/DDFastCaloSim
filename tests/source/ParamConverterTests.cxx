#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <gtest/gtest.h>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "TestConfig/ParamConverterTestsConfig.h"
#include "TestHelpers/IOManager.h"

TEST(ParamConverterTests, ParamConverterTest)
{
  // Create the output directory for the test
  const std::string output_dir =
      TestHelpers::IOManager::create_test_output_dir();

  // Build command
  const std::string cmd = ParamConverterTestsConfig::converter_cmd(output_dir);
  std::cout << "Executing command: " << cmd << std::endl;

  int status = system(cmd.c_str());
  ASSERT_EQ(status, 0) << "Executable failed: " << cmd;

  const std::string output_file =
      ParamConverterTestsConfig::converter_output_path(output_dir);

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

  fout.Close();
}
