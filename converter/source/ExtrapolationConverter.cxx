#include "FastCaloSimConverter/ExtrapolationConverter.h"

#include <TTree.h>

#include "FastCaloSim/Core/TFCSExtrapolationState.h"
#include "FastCaloSim/Geometry/Cell.h"

ExtrapolationConverter::ExtrapolationConverter(int m_n_layers)
    : m_n_layers(m_n_layers)
{
  /// @brief Calorimeter face position for each incident particle
  m_newTTC_IDCaloBoundary_eta = new std::vector<float>;
  m_newTTC_IDCaloBoundary_phi = new std::vector<float>;
  m_newTTC_IDCaloBoundary_r = new std::vector<float>;
  m_newTTC_IDCaloBoundary_z = new std::vector<float>;
  m_newTTC_Angle3D = new std::vector<float>;
  m_newTTC_AngleEta = new std::vector<float>;

  /// @brief Entrance position of for each calorimeter layer for each particle
  m_newTTC_entrance_eta = new std::vector<std::vector<float>>;
  m_newTTC_entrance_phi = new std::vector<std::vector<float>>;
  m_newTTC_entrance_r = new std::vector<std::vector<float>>;
  m_newTTC_entrance_z = new std::vector<std::vector<float>>;
  m_newTTC_entrance_detaBorder = new std::vector<std::vector<float>>;
  m_newTTC_entrance_OK = new std::vector<std::vector<bool>>;

  /// @brief Middle position of for each calorimeter layer for each particle
  m_newTTC_mid_eta = new std::vector<std::vector<float>>;
  m_newTTC_mid_phi = new std::vector<std::vector<float>>;
  m_newTTC_mid_r = new std::vector<std::vector<float>>;
  m_newTTC_mid_z = new std::vector<std::vector<float>>;
  m_newTTC_mid_detaBorder = new std::vector<std::vector<float>>;
  m_newTTC_mid_OK = new std::vector<std::vector<bool>>;

  /// @brief Exit position of for each calorimeter layer for each particle
  m_newTTC_back_eta = new std::vector<std::vector<float>>;
  m_newTTC_back_phi = new std::vector<std::vector<float>>;
  m_newTTC_back_r = new std::vector<std::vector<float>>;
  m_newTTC_back_z = new std::vector<std::vector<float>>;
  m_newTTC_back_detaBorder = new std::vector<std::vector<float>>;
  m_newTTC_back_OK = new std::vector<std::vector<bool>>;
}

ExtrapolationConverter::~ExtrapolationConverter()
{
  delete m_newTTC_IDCaloBoundary_eta;
  delete m_newTTC_IDCaloBoundary_phi;
  delete m_newTTC_IDCaloBoundary_r;
  delete m_newTTC_IDCaloBoundary_z;
  delete m_newTTC_Angle3D;

  delete m_newTTC_entrance_eta;
  delete m_newTTC_entrance_phi;
  delete m_newTTC_entrance_r;
  delete m_newTTC_entrance_z;
  delete m_newTTC_entrance_detaBorder;

  delete m_newTTC_mid_eta;
  delete m_newTTC_mid_phi;
  delete m_newTTC_mid_r;
  delete m_newTTC_mid_z;
  delete m_newTTC_mid_detaBorder;

  delete m_newTTC_back_eta;
  delete m_newTTC_back_phi;
  delete m_newTTC_back_r;
  delete m_newTTC_back_z;
  delete m_newTTC_back_detaBorder;
}

void ExtrapolationConverter::createBranches(TTree* outTree)
{
  if (!outTree) {
    std::cerr
        << "ExtrapolationConverter::createBranches ERROR: outTree is null!\n";
    return;
  }

  /// @brief Calorimeter face position for each incident particle
  outTree->Branch("newTTC_IDCaloBoundary_eta", &m_newTTC_IDCaloBoundary_eta);
  outTree->Branch("newTTC_IDCaloBoundary_phi", &m_newTTC_IDCaloBoundary_phi);
  outTree->Branch("newTTC_IDCaloBoundary_r", &m_newTTC_IDCaloBoundary_r);
  outTree->Branch("newTTC_IDCaloBoundary_z", &m_newTTC_IDCaloBoundary_z);
  outTree->Branch("newTTC_Angle3D", &m_newTTC_Angle3D);

  /// @brief Entrance position of for each calorimeter layer for each particle
  outTree->Branch("newTTC_entrance_eta", &m_newTTC_entrance_eta);
  outTree->Branch("newTTC_entrance_phi", &m_newTTC_entrance_phi);
  outTree->Branch("newTTC_entrance_r", &m_newTTC_entrance_r);
  outTree->Branch("newTTC_entrance_z", &m_newTTC_entrance_z);
  outTree->Branch("newTTC_entrance_detaBorder", &m_newTTC_entrance_detaBorder);
  outTree->Branch("newTTC_entrance_OK", &m_newTTC_entrance_OK);

  /// @brief Middle position of for each calorimeter layer for each particle
  outTree->Branch("newTTC_mid_eta", &m_newTTC_mid_eta);
  outTree->Branch("newTTC_mid_phi", &m_newTTC_mid_phi);
  outTree->Branch("newTTC_mid_r", &m_newTTC_mid_r);
  outTree->Branch("newTTC_mid_z", &m_newTTC_mid_z);
  outTree->Branch("newTTC_mid_detaBorder", &m_newTTC_mid_detaBorder);
  outTree->Branch("newTTC_mid_OK", &m_newTTC_mid_OK);

  /// @brief Exit position of for each calorimeter layer for each particle
  outTree->Branch("newTTC_back_eta", &m_newTTC_back_eta);
  outTree->Branch("newTTC_back_phi", &m_newTTC_back_phi);
  outTree->Branch("newTTC_back_r", &m_newTTC_back_r);
  outTree->Branch("newTTC_back_z", &m_newTTC_back_z);
  outTree->Branch("newTTC_back_detaBorder", &m_newTTC_back_detaBorder);
  outTree->Branch("newTTC_back_OK", &m_newTTC_back_OK);
}

void ExtrapolationConverter::fillEvent(
    const std::vector<TFCSExtrapolationState>& extrapolations)
{
  // 1) Clear for the new event
  m_newTTC_IDCaloBoundary_eta->clear();
  m_newTTC_IDCaloBoundary_phi->clear();
  m_newTTC_IDCaloBoundary_r->clear();
  m_newTTC_IDCaloBoundary_z->clear();
  m_newTTC_Angle3D->clear();
  m_newTTC_AngleEta->clear();

  m_newTTC_entrance_eta->clear();
  m_newTTC_entrance_phi->clear();
  m_newTTC_entrance_r->clear();
  m_newTTC_entrance_z->clear();
  m_newTTC_entrance_detaBorder->clear();
  m_newTTC_entrance_OK->clear();

  m_newTTC_mid_eta->clear();
  m_newTTC_mid_phi->clear();
  m_newTTC_mid_r->clear();
  m_newTTC_mid_z->clear();
  m_newTTC_mid_detaBorder->clear();
  m_newTTC_mid_OK->clear();

  m_newTTC_back_eta->clear();
  m_newTTC_back_phi->clear();
  m_newTTC_back_r->clear();
  m_newTTC_back_z->clear();
  m_newTTC_back_detaBorder->clear();
  m_newTTC_back_OK->clear();

  // 2) Loop over each TFCSExtrapolationState
  for (const auto& state : extrapolations) {
    m_newTTC_IDCaloBoundary_eta->push_back(float(state.IDCaloBoundary_eta()));
    m_newTTC_IDCaloBoundary_phi->push_back(float(state.IDCaloBoundary_phi()));
    m_newTTC_IDCaloBoundary_r->push_back(float(state.IDCaloBoundary_r()));
    m_newTTC_IDCaloBoundary_z->push_back(float(state.IDCaloBoundary_z()));
    m_newTTC_Angle3D->push_back(float(state.IDCaloBoundary_Angle3D()));
    m_newTTC_AngleEta->push_back(float(state.IDCaloBoundary_AngleEta()));

    std::vector<float> layerEta, layerPhi, layerR, layerZ, layerDetaBorder;
    std::vector<bool> layerOK;

    for (int layer = 0; layer < m_n_layers; layer++) {
      layerEta.push_back(state.eta(layer, Cell::SubPos::ENT));
      layerPhi.push_back(state.phi(layer, Cell::SubPos::ENT));
      layerR.push_back(state.r(layer, Cell::SubPos::ENT));
      layerZ.push_back(state.z(layer, Cell::SubPos::ENT));
      layerDetaBorder.push_back(state.detaBorder(layer, Cell::SubPos::ENT));
      layerOK.push_back(state.OK(layer, Cell::SubPos::ENT));
    }

    m_newTTC_entrance_eta->push_back(std::move(layerEta));
    m_newTTC_entrance_phi->push_back(std::move(layerPhi));
    m_newTTC_entrance_r->push_back(std::move(layerR));
    m_newTTC_entrance_z->push_back(std::move(layerZ));
    m_newTTC_entrance_detaBorder->push_back(std::move(layerDetaBorder));
    m_newTTC_entrance_OK->push_back(std::move(layerOK));
  }
}
