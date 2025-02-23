// -- DDFastCaloSim includes
#include "DDFastCaloSim/ParametrizationModel.h"

#include "DDFastCaloSim/ParametrizationWriter.h"
#include "DDFastCaloSim/TrackMsg.h"

// -- Geant4 includes
#include <G4FastStep.hh>
#include <G4FastTrack.hh>
#include <G4Geantino.hh>

dd4hep::sim::ParametrizationModel::ParametrizationModel(
    dd4hep::sim::Geant4Context* context, const std::string& name)
    : dd4hep::sim::FastCaloSimModel(context, name)
{
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
void dd4hep::sim::ParametrizationModel::modelShower(const G4FastTrack& aTrack,
                                                    G4FastStep& aStep)
{
  // Get Geant4 primary track
  const G4Track* track = aTrack.GetPrimaryTrack();

  // Print track information
  auto msg = print_track(track);
  printout(INFO, "ParametrizationModel", msg);

  // Make sure this hook is only called with geantinos
  if (track->GetDefinition() != G4Geantino::GeantinoDefinition()) {
    printout(ERROR,
             "ParametrizationModel",
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
             "ParametrizationModel",
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

  TestHelpers::Track trk(step_vector);
  m_transportTracks.add(trk);

  // Print number of steps
  printout(
      INFO, "ParametrizationModel", "Number of steps: %d", step_vector.size());

  TFCSExtrapolationState extrapol_state;
  /// TODO: m_extrapolationTool.extrapolate(step_vector, state);

  auto collector = dynamic_cast<dd4hep::sim::ParamCollector*>(
      context()->eventAction().get("ParamCollector"));
  if (collector) {
    collector->get_event_data().extrapolations.push_back(extrapol_state);
  } else {
    printout(ERROR,
             "ParametrizationModel",
             "ParamCollector not found in event action sequence!");
  }

  // Kill initial geantino
  killParticle(aStep, 0);

  return;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(ParametrizationModel)
