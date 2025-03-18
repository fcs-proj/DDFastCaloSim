#include <iostream>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "FastCaloSimConverter/ExtrapolationConverter.h"
#include "FastCaloSimConverter/InputLoader.h"
#include "TFile.h"
#include "TTree.h"

auto main(int argc, char* argv[]) -> int
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << " <model_input> <dd4hep_input> <output_file>" << std::endl;
    return 1;
  }
  std::string model_input = argv[1];
  std::string dd4hep_input = argv[2];
  std::string output_file = argv[3];

  InputLoader loader;

  // Get trees from files with specified branch names
  TTree* model_tree = loader.get_tree(model_input, "param_info");
  TTree* dd4hep_tree = loader.get_tree(dd4hep_input, "events");

  // Make sure trees have identical number of entries
  if (model_tree->GetEntries() != dd4hep_tree->GetEntries()) {
    std::cerr << "Error: Trees have different number of entries" << std::endl;
    return 1;
  }
  // Number of events to process
  int n_entries = model_tree->GetEntries();

  // Create output file
  TFile* fout = TFile::Open(output_file.c_str(), "RECREATE");
  TTree* outTree =
      new TTree("FCS_ParametrizationInput", "FCS_ParametrizationInput");

  // Number of layers in the calorimeter
  // -> Crashes if set to >0 as extrapolation states are not initialized
  int n_layers = 0;

  // Initialize extrapolation converter
  ExtrapolationConverter converter {n_layers};
  converter.create_branches(outTree);
  converter.set_addresses(model_tree);

  // Fill the output tree
  for (Long64_t i = 0; i < n_entries; i++) {
    model_tree->GetEntry(i);
    outTree->Fill();
  }

  // Write out the new TTree
  fout->cd();
  outTree->Write();

  // Cleanup
  fout->Close();

  std::cout << "[ExtrapolationConverter] Done. Wrote " << n_entries
            << " entries." << std::endl;
  std::cout << "[ExtrapolationConverter] Output file: " << output_file
            << std::endl;
  return 0;
}
