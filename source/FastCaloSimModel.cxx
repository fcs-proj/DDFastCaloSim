#include "DDFastCaloSim/FastCaloSimModel.h"

// -- Framework includes
#include <DD4hep/Printout.h>

// -- Geant4 includes
#include <G4FastStep.hh>

#include "G4Event.hh"
#include "G4EventManager.hh"

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
  truth.set_pdgid(track->GetDefinition()->GetPDGEncoding());

  // Set the kinematics of the FastCaloSim truth state
  truth.SetPtEtaPhiM(track->GetMomentum().perp(),
                     track->GetMomentum().eta(),
                     track->GetMomentum().phi(),
                     track->GetDefinition()->GetPDGMass());

  // Set the vertex of the FastCaloSim truth state
  truth.set_vertex(track->GetPosition().x(),
                   track->GetPosition().y(),
                   track->GetPosition().z());

  printout(
      INFO, "FastSimModel", "Received particle with pid=%d", truth.pdgid());

  printout(INFO,
           "FastSimModel",
           "Particle has position (%.2f, %.2f, %.2f) eta=%.2f "
           "phi=%.2f r=%.2f ekin=%.2f",
           truth.X(),
           truth.Y(),
           truth.Z(),
           truth.Eta(),
           truth.Phi(),
           truth.Perp(),
           truth.Ekin());

  printout(INFO,
           "FastSimModel",
           "Particle has momentum (%.2f, %.2f, %.2f) p=%.2f "
           "pt=%.2f eta=%.2f",
           track->GetMomentum().x(),
           track->GetMomentum().y(),
           track->GetMomentum().z(),
           track->GetMomentum().mag(),
           track->GetMomentum().perp(),
           track->GetMomentum().eta());

  // Kill particle
  aStep.KillPrimaryTrack();
  aStep.ProposePrimaryTrackPathLength(0.0);
  return;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSimModel)
