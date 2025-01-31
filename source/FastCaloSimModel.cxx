#include "DDFastCaloSim/FastCaloSimModel.h"

#include "DDFastCaloSim/TrackMsg.h"

// -- Framework includes
#include <DD4hep/Printout.h>

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
{
}
bool dd4hep::sim::FastCaloSimModel::check_trigger(const G4FastTrack& track)
{
  std::cout << "Called fast simulation trigger" << std::endl;
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

  // Kill particle
  aStep.KillPrimaryTrack();
  aStep.ProposePrimaryTrackPathLength(0.0);
  return;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSimModel)
