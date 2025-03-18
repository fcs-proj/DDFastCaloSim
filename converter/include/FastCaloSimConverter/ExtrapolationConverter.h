#pragma once

#include <vector>

#include <RtypesCore.h>

class TTree;
class TFCSExtrapolationState;

class ExtrapolationConverter
{
public:
  ExtrapolationConverter(int m_n_layers);
  ~ExtrapolationConverter();

  /// @brief Creates branches in the output TTree for extrapolation data
  void create_branches(TTree* outTree);

  /// @brief Fills event data from extrapolation states vector
  void fill_event(const std::vector<TFCSExtrapolationState>& extrapolations);

  /// @brief Processes a single entry from the input tree
  void process_entry(Long64_t entry, TTree* input_tree);

  /// @brief Sets branch addresses in the input TTree
  void set_addresses(TTree* inputTree);

private:
  // Number of calorimeter layers
  unsigned int m_n_layers;

  // Branches of the input tree
  std::vector<TFCSExtrapolationState>* m_extrapolations = nullptr;

  // Branches of the output tree
  std::vector<float>* m_newTTC_IDCaloBoundary_eta = nullptr;
  std::vector<float>* m_newTTC_IDCaloBoundary_phi = nullptr;
  std::vector<float>* m_newTTC_IDCaloBoundary_r = nullptr;
  std::vector<float>* m_newTTC_IDCaloBoundary_z = nullptr;
  std::vector<float>* m_newTTC_Angle3D = nullptr;
  std::vector<float>* m_newTTC_AngleEta = nullptr;

  std::vector<std::vector<float>>* m_newTTC_entrance_eta = nullptr;
  std::vector<std::vector<float>>* m_newTTC_entrance_phi = nullptr;
  std::vector<std::vector<float>>* m_newTTC_entrance_r = nullptr;
  std::vector<std::vector<float>>* m_newTTC_entrance_z = nullptr;
  std::vector<std::vector<float>>* m_newTTC_entrance_detaBorder = nullptr;
  std::vector<std::vector<bool>>* m_newTTC_entrance_OK = nullptr;

  std::vector<std::vector<float>>* m_newTTC_mid_eta = nullptr;
  std::vector<std::vector<float>>* m_newTTC_mid_phi = nullptr;
  std::vector<std::vector<float>>* m_newTTC_mid_r = nullptr;
  std::vector<std::vector<float>>* m_newTTC_mid_z = nullptr;
  std::vector<std::vector<float>>* m_newTTC_mid_detaBorder = nullptr;
  std::vector<std::vector<bool>>* m_newTTC_mid_OK = nullptr;

  std::vector<std::vector<float>>* m_newTTC_back_eta = nullptr;
  std::vector<std::vector<float>>* m_newTTC_back_phi = nullptr;
  std::vector<std::vector<float>>* m_newTTC_back_r = nullptr;
  std::vector<std::vector<float>>* m_newTTC_back_z = nullptr;
  std::vector<std::vector<float>>* m_newTTC_back_detaBorder = nullptr;
  std::vector<std::vector<bool>>* m_newTTC_back_OK = nullptr;
};
