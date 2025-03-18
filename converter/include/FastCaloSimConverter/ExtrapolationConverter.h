#pragma once

#include <vector>

#include <RtypesCore.h>

class TTree;
class TTreeReader;
#include <TTreeReaderValue.h>

class TFCSExtrapolationState;

class ExtrapolationConverter
{
public:
  ExtrapolationConverter(int n_layers, TTreeReader* reader, TTree* outTree);
  ~ExtrapolationConverter();

  /// @brief Initializes all data containers
  void initialize_data_containers();

  /// @brief Creates branches in the output TTree for extrapolation data
  void create_branches(TTree* outTree);

  /// @brief Fills event data from extrapolation states vector
  void fill_event(const std::vector<TFCSExtrapolationState>& extrapolations);

  /// @brief Processes current entry from a TTreeReader
  void process_entry();

private:
  // Number of calorimeter layers
  unsigned int m_n_layers;

  // TTreeReader and branches for the input tree
  TTreeReader* m_reader = nullptr;
  TTreeReaderValue<std::vector<TFCSExtrapolationState>>*
      m_extrapolations_reader = nullptr;

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
