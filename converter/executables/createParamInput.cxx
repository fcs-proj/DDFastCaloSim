#include <iostream>

#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "FastCaloSimConverter/ExtrapolationConverter.h"

auto main(int argc, char* argv[]) -> int
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <input_param_output.root> <output_converter_output.root>"
              << std::endl;
    return 1;
  }

  std::string param_input = argv[1];
  std::string output_file = argv[2];

  // 1) Open input file, retrieve TTree
  TFile* fin = TFile::Open(param_input.c_str(), "READ");
  if (!fin || fin->IsZombie()) {
    std::cerr << "Error: Failed to open input ROOT file: " << param_input
              << std::endl;
    return 1;
  }

  TTree* inTree = dynamic_cast<TTree*>(fin->Get("param_info"));
  if (!inTree) {
    std::cerr << "Error: TTree 'param_info' not found in ROOT file: "
              << param_input << std::endl;
    return 1;
  }

  // Set branch address for "extrapolations"
  std::vector<TFCSExtrapolationState>* extrapolations = nullptr;
  inTree->SetBranchAddress("extrapolations", &extrapolations);

  // 2) Create output file + TTree
  TFile* fout = TFile::Open(output_file.c_str(), "RECREATE");
  if (!fout || fout->IsZombie()) {
    std::cerr << "Error: Failed to create output ROOT file: " << output_file
              << std::endl;
    return 1;
  }

  TTree* outTree =
      new TTree("FCS_ParametrizationInput", "FCS_ParametrizationInput");

  // Number of layers in the calorimeter
  // -> Crashes if set to >0 as extrapolation states are not initialized
  int n_layers = 0;

  // 3) Create an instance of our ExtrapolationConverter
  ExtrapolationConverter converter {n_layers};

  // 4) Let the converter create branches for newTTC stuff in 'outTree'
  converter.createBranches(outTree);

  Long64_t nentries = inTree->GetEntries();
  for (Long64_t i = 0; i < nentries; i++) {
    inTree->GetEntry(i);

    // Fill the converter's data from extrapolations
    converter.fillEvent(*extrapolations);

    // Fill this row in the output TTree
    outTree->Fill();
  }

  // 7) Write out the new TTree
  fout->cd();
  outTree->Write();

  // Cleanup
  fout->Close();
  fin->Close();

  std::cout << "Done, wrote " << nentries << " entries to " << output_file
            << std::endl;
  return 0;
}
