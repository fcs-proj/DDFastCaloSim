#pragma once

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "TTree.h"

/// @brief Struct to store event data recorded in modelShower()
/// for writing input required for parametrization to file
struct EventData
{
  std::vector<TFCSExtrapolationState> extrapolations;

  /// Bind struct fields to a ROOT TTree
  void bind_to_tree(TTree* tree)
  {
    tree->Branch("extrapolations", &extrapolations);
  }

  /// Clears data after writing
  void clear() { extrapolations.clear(); }
};
