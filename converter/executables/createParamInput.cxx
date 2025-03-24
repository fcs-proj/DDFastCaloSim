#include <iostream>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "FastCaloSimConverter/ExtrapolationConverter.h"
#include "FastCaloSimConverter/InputLoader.h"
#include "FastCaloSimConverter/HitConverter.h"

// ROOT headers
#include "ROOT/TBufferMerger.hxx"
#include "ROOT/TTreeProcessorMT.hxx"
#include "TROOT.h"
#include "TTree.h"
#include "TTreeReader.h"

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

  // Enable multi-threading in ROOT
  ROOT::EnableImplicitMT();

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

  // Create a buffer merger for thread-safe writing
  auto merger =
      std::make_unique<ROOT::TBufferMerger>(output_file.c_str(), "RECREATE");

  // Number of layers in the calorimeter
  // -> Crashes if set to >0 as extrapolation states are not initialized
  int n_layers = 0;

  // Define the processing function that each thread will execute
  auto processFunction = [&](TTreeReader& reader)
  {
    // Get a file from the merger to create a local tree
    auto myFile = merger->GetFile();
    TTree* outTree =
        new TTree("FCS_ParametrizationInput", "FCS_ParametrizationInput");

    // Create a thread-local ExtrapolationConverter
    ExtrapolationConverter extrap_converter(n_layers, &reader, outTree);
    HitConverter hit_converter(dd4hep_input, outTree);

    // Process entries in this thread's chunk
    while (reader.Next()) {
      Long64_t entryIndex = reader.GetCurrentEntry();
      extrap_converter.process_entry();
      hit_converter.process_entry(entryIndex);
      outTree->Fill();
    }

    // Write the local tree to the merged output
    myFile->Write();
  };

  // Create a TTreeProcessorMT to process the model tree in parallel
  ROOT::TTreeProcessorMT processor(*model_tree);
  processor.Process(processFunction);

  std::cout << "[createParamInput] Done. Wrote " << n_entries << " entries."
            << std::endl;
  std::cout << "[createParamInput] Output file: " << output_file << std::endl;
  return 0;
}
