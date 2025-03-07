#pragma once

#include <vector>
class TTree;
class TFCSExtrapolationState;

class ExtrapolationConverter
{
public:
  ExtrapolationConverter(int m_n_layers);
  ~ExtrapolationConverter();

  void createBranches(TTree* outTree);

  void fillEvent(const std::vector<TFCSExtrapolationState>& extrapolations);

private:
  // Number of calorimeter layers
  unsigned int m_n_layers;

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
