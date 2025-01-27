#include "DDFastCaloSim/FastCaloSim.h"

#include "G4Event.hh"
#include "G4EventManager.hh"

dd4hep::sim::FastCaloSim::FastCaloSim(dd4hep::sim::Geant4Context* context,
                                      const std::string& name)
    : Geant4FastSimShowerModel(context, name)
{
}
bool dd4hep::sim::FastCaloSim::check_trigger(const G4FastTrack& track)
{
  std::cout << "Called fast simulation trigger" << std::endl;
  return true;
}

void dd4hep::sim::FastCaloSim::modelShower(const G4FastTrack& aTrack,
                                           G4FastStep& aStep)
{
  // Kill particle
  aStep.KillPrimaryTrack();
  aStep.SetPrimaryTrackPathLength(0.0);
  return;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSim)
