#include "DDFastCaloSim/FastCaloSimModel.h"

#include "DDFastCaloSim/FastCaloSimWriter.h"
#include "DDFastCaloSim/TrackMsg.h"

// -- Framework includes
#include <DD4hep/Printout.h>
#include <DDG4/Geant4Action.h>

// -- Geant4 includes
#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4FastStep.hh>

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
    , m_use_simplified_geo(false)
    , m_transport_limit_volume("")
    , m_max_transport_steps(100)
    , m_transport_output("")
{
  // If set, the transport tracks will be serialized to this JSON file
  declareProperty("TransportOutputFile", m_transport_output);
  // Boolean flag to use simplified geometry
  declareProperty("UseSimplifiedGeo", m_use_simplified_geo);
  // Maximum number of transport steps
  declareProperty("MaxTransportSteps", m_max_transport_steps);
  // Name of the transport limit volume
  declareProperty("TransportLimitVolume", m_transport_limit_volume);

  /// TODO: Set the CaloGeo* geometry for extrapolation
  m_extrapolationTool.set_geometry(nullptr);
}
bool dd4hep::sim::FastCaloSimModel::check_trigger(const G4FastTrack& track)
{
  // Ideally this would happen as a begin of run action
  // Needs to be initialized only once, but geometry not available at
  // construction time. TODO: fix this
  if (!m_transport_init) {
    m_transportTool.setUseSimplifiedGeo(m_use_simplified_geo);
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
  // Print track information
  auto msg = print_track(track);
  printout(INFO, "FastCaloSimModel", msg);

  // Perform the track transport
  std::vector<G4FieldTrack> step_vector = m_transportTool.transport(*track);
  TestHelpers::Track trk(step_vector);
  m_transportTracks.add(trk);

  auto writer = dynamic_cast<dd4hep::sim::FastCaloSimWriter*>(
      context()->eventAction().get("FastCaloSimWriter"));
  if (writer) {
    writer->addData(1);  // add your float value (or computed value)
  } else {
    printout(ERROR,
             "FastCaloSimModel",
             "FastCaloSimWriter not found in event action sequence!");
  }

  // TFCSExtrapolationState extrap;
  // m_extrapolationTool.extrapolate(extrap, &truth, step_vector);

  // Print the step vector
  printout(
      INFO, "FastCaloSimModel", "Step vector size: %d", step_vector.size());

  killParticle(aStep, 0);

  return;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSimModel)
