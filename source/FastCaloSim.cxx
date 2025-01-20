#include "DDFastCaloSim/FastCaloSim.h"
#include "G4EventManager.hh"
#include "G4Event.hh"

dd4hep::sim::FastCaloSim::FastCaloSim(dd4hep::sim::Geant4Context* context, const std::string& nam)
      : Geant4FastSimShowerModel(context, nam)
    {}
  bool dd4hep::sim::FastCaloSim::check_trigger(const G4FastTrack& track) {
    return true;
  }

     void dd4hep::sim::FastCaloSim::modelShower(const G4FastTrack& aTrack, G4FastStep& aStep) {
        return;
    }


#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSim)
