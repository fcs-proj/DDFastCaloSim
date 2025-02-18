#include "DDFastCaloSim/FastCaloSimModel.h"

#include "DDFastCaloSim/TrackMsg.h"

// -- Framework includes
#include <DD4hep/Printout.h>

// -- Geant4 includes
#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4FastStep.hh>
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
    , m_use_simplified_geo(false)
    , m_transport_limit_volume("")
    , m_max_transport_steps(100)
    , m_transport_output("")
    , m_param_pdgId(-1)
{
  // If set, the transport tracks will be serialized to this JSON file
  declareProperty("TransportOutputFile", m_transport_output);
  // Boolean flag to use simplified geometry
  declareProperty("UseSimplifiedGeo", m_use_simplified_geo);
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

  // Print track information
  auto msg = print_track(track);
  printout(INFO, "FastCaloSimModel", msg);

  // Hook if we only want to generate the parametrization input
  if (m_param_pdgId != -1) {
    createParametrizationInput(aTrack, aStep);
    return;
  }

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
  TestHelpers::Track trk(step_vector);
  m_transportTracks.add(trk);

  // Print the step vector
  printout(
      INFO, "FastCaloSimModel", "Step vector size: %d", step_vector.size());

  killParticle(aStep, 0);

  return;
}

/**
 * @brief Hook to create the input for the FastCaloSim parametrization
 *
 * To generate the input for the FastCaloSim parametrization:
 * - Shoot non-interacting particles (geantinos) to the calorimeter face
 * - Replace the geantino with the particle to be parametrized
 *   (with consistent position, energy, and direction)
 * - Record the transport of the particle through the calorimeter
 * - Continue the Geant4 shower simulation to record G4 info
 */

void dd4hep::sim::FastCaloSimModel::createParametrizationInput(
    const G4FastTrack& aTrack, G4FastStep& aStep)
{
  printout(INFO, "FastCaloSimModel", "Running in parametrization mode...");

  // Get Geant4 primary track
  const G4Track* track = aTrack.GetPrimaryTrack();

  // Make sure this hook is only called with geantinos
  if (track->GetDefinition() != G4Geantino::GeantinoDefinition()) {
    printout(ERROR,
             "FastCaloSimModel",
             "Received particle is not a geantino. Aborting...");
    abort();
  }

  // Replace the geantino with exactly one secondary
  aStep.SetNumberOfSecondaryTracks(1);

  // Build the secondary track
  G4ParticleMomentum direction(aTrack.GetPrimaryTrackLocalDirection());

  // Find the requested particle for the parametrization
  G4ParticleDefinition* ptcl_def =
      G4ParticleTable::GetParticleTable()->FindParticle(m_param_pdgId);

  if (!ptcl_def) {
    printout(ERROR,
             "FastCaloSimModel",
             "Failed to find particle with PDG ID %d. Aborting...",
             m_param_pdgId);
    abort();
  }
  // Sets the particle type, energy and direction
  G4DynamicParticle particle(
      ptcl_def, direction, aTrack.GetPrimaryTrack()->GetKineticEnergy());

  // Create the secondary track, this is the particle to be parametrized
  G4Track* param_ptcl =
      aStep.CreateSecondaryTrack(particle,
                                 aTrack.GetPrimaryTrackLocalPosition(),
                                 aTrack.GetPrimaryTrack()->GetGlobalTime());

  // Do transport (and extrapolation) and write out information
  std::vector<G4FieldTrack> step_vector =
      m_transportTool.transport(*param_ptcl);

  /// TODO: Implement extrapolation and writer

  // Kill initial geantino
  killParticle(aStep, 0);
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSimModel)
