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
  delete m_newTTC_AngleEta;

  delete m_newTTC_entrance_eta;
  delete m_newTTC_entrance_phi;
  delete m_newTTC_entrance_r;
  delete m_newTTC_entrance_z;
  delete m_newTTC_entrance_detaBorder;
  delete m_newTTC_entrance_OK;

  delete m_newTTC_mid_eta;
  delete m_newTTC_mid_phi;
  delete m_newTTC_mid_r;
  delete m_newTTC_mid_z;
  delete m_newTTC_mid_detaBorder;
  delete m_newTTC_mid_OK;

  delete m_newTTC_back_eta;
  delete m_newTTC_back_phi;
  delete m_newTTC_back_r;
  delete m_newTTC_back_z;
  delete m_newTTC_back_detaBorder;
  delete m_newTTC_back_OK;
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
  outTree->Branch("newTTC_AngleEta", &m_newTTC_AngleEta);

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

    std::vector<std::vector<float>> eta(3), phi(3), r(3), z(3), detaBorder(3);
    std::vector<std::vector<bool>> ok(3);

    for (int layer = 0; layer < m_n_layers; layer++) {
      for (int pos = 0; pos < 3; pos++)
      {  // Loop over SubPos (ENT=1, MID=0, EXT=2)
        eta[pos].push_back(state.eta(layer, static_cast<Cell::SubPos>(pos)));
        phi[pos].push_back(state.phi(layer, static_cast<Cell::SubPos>(pos)));
        r[pos].push_back(state.r(layer, static_cast<Cell::SubPos>(pos)));
        z[pos].push_back(state.z(layer, static_cast<Cell::SubPos>(pos)));
        detaBorder[pos].push_back(
            state.detaBorder(layer, static_cast<Cell::SubPos>(pos)));
        ok[pos].push_back(state.OK(layer, static_cast<Cell::SubPos>(pos)));
      }
    }

    m_newTTC_entrance_eta->push_back(std::move(eta[1]));
    m_newTTC_entrance_phi->push_back(std::move(phi[1]));
    m_newTTC_entrance_r->push_back(std::move(r[1]));
    m_newTTC_entrance_z->push_back(std::move(z[1]));
    m_newTTC_entrance_detaBorder->push_back(std::move(detaBorder[1]));
    m_newTTC_entrance_OK->push_back(std::move(ok[1]));

    m_newTTC_mid_eta->push_back(std::move(eta[0]));
    m_newTTC_mid_phi->push_back(std::move(phi[0]));
    m_newTTC_mid_r->push_back(std::move(r[0]));
    m_newTTC_mid_z->push_back(std::move(z[0]));
    m_newTTC_mid_detaBorder->push_back(std::move(detaBorder[0]));
    m_newTTC_mid_OK->push_back(std::move(ok[0]));

    m_newTTC_back_eta->push_back(std::move(eta[2]));
    m_newTTC_back_phi->push_back(std::move(phi[2]));
    m_newTTC_back_r->push_back(std::move(r[2]));
    m_newTTC_back_z->push_back(std::move(z[2]));
    m_newTTC_back_detaBorder->push_back(std::move(detaBorder[2]));
    m_newTTC_back_OK->push_back(std::move(ok[2]));
  }
}
