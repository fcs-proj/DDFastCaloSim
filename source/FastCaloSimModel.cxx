#include "DDFastCaloSim/FastCaloSimModel.h"

#include "DDFastCaloSim/TrackMsg.h"

// -- Framework includes
#include <DD4hep/Printout.h>

// -- Geant4 includes
#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4FastStep.hh>
#include <G4GDMLParser.hh>
#include <G4Geantino.hh>

// -- FastCaloSim includes
#include "FastCaloSim/Core/TFCSTruthState.h"

dd4hep::sim::FastCaloSimModel::FastCaloSimModel(
    dd4hep::sim::Geant4Context* context, const std::string& name)
    : Geant4FastSimShowerModel(context, name)
    , m_parametrization()
    , m_transportTool()
    , m_extrapolationTool()
    , m_transport_init(false)
    , m_transportTracks()
    , m_simplified_geo_path("")
    , m_transport_limit_volume("")
    , m_max_transport_steps(100)
    , m_transport_output("")
    , m_param_pdgId(-1)
{
  // If set, the transport tracks will be serialized to this JSON file
  declareProperty("TransportOutputFile", m_transport_output);
  // Boolean flag to use simplified geometry
  declareProperty("SimplifiedGeoPath", m_simplified_geo_path);
  // Maximum number of transport steps
  declareProperty("MaxTransportSteps", m_max_transport_steps);
  // Name of the transport limit volume
  declareProperty("TransportLimitVolume", m_transport_limit_volume);
  // If set, the model will generate input for the parametrization
  declareProperty("ParametrizationPDG", m_param_pdgId);

  /// TODO: Set the CaloGeo* geometry for extrapolation
  m_extrapolationTool.set_geometry(nullptr);
}
bool dd4hep::sim::FastCaloSimModel::check_trigger(const G4FastTrack& track)
{
  /// @TODO:
  // Ideally this would happen as a begin of run action
  // Needs to be initialized only once, but geometry not available at
  // construction time

  if (!m_transport_init) {
    if (!m_simplified_geo_path.empty()) {
      // Load simplified geometry if set
      G4GDMLParser parser;
      parser.Read(m_simplified_geo_path, false);
      m_transportTool.setUseSimplifiedGeo(true);
    } else {
      // Otherwise use the full geometry
      m_transportTool.setUseSimplifiedGeo(false);
    }

    m_transportTool.setMaxSteps(m_max_transport_steps);

    if (m_transport_limit_volume.empty()) {
      printout(ERROR,
               "FastCaloSimModel",
               "Transport limit volume not set. Aborting...");
      abort();
    }
    m_transportTool.setTransportLimitVolume(m_transport_limit_volume);
    m_transportTool.initializePropagator();
    m_transport_init = true;
  }

  return true;
}

void dd4hep::sim::FastCaloSimModel::modelShower(const G4FastTrack& aTrack,
                                                G4FastStep& aStep)
{
  // Get Geant4 primary track
  const G4Track* track = aTrack.GetPrimaryTrack();

  // Print track information
  auto msg = print_track(track);
  printout(INFO, "FastCaloSimModel", msg);

  // Set the FastCaloSim truth state
  TFCSTruthState truth;

  // Set the PDG ID of the particle
  truth.set_pdgid(track->GetDefinition()->GetPDGEncoding());

  // Set the vertex of the particle
  truth.set_vertex(track->GetPosition().x(),
                   track->GetPosition().y(),
                   track->GetPosition().z());
  // Set the momentum of the particle
  truth.SetPtEtaPhiM(track->GetMomentum().perp(),
                     track->GetMomentum().eta(),
                     track->GetMomentum().phi(),
                     track->GetDefinition()->GetPDGMass());

  // Perform the track transport
  std::vector<G4FieldTrack> step_vector = m_transportTool.transport(*track);

  // Print the step vector
  printout(
      INFO, "FastCaloSimModel", "Step vector size: %d", step_vector.size());

  killParticle(aStep, 0);

  return;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSimModel)
