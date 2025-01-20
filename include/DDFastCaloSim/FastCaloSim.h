#ifndef FastCaloSim_HH
#define FastCaloSim_HH

#include <G4String.hh>                // for G4String
#include <G4Types.hh>                 // for G4bool
#include "G4VFastSimulationModel.hh"  // for G4VFastSimulationModel
class G4FastStep;
class G4FastTrack;
class G4ParticleDefinition;
class G4Region;

#include <DDG4/Geant4FastSimShowerModel.h>

#include <G4FastStep.hh>
#include <G4FastTrack.hh>
#include <G4Track.hh>
#include <G4FastHit.hh>


/**
 * @brief Register particles at calorimeter layer
 *
 */
namespace dd4hep{
  namespace sim{
class FastCaloSim :  public dd4hep::sim::Geant4FastSimShowerModel
{
 public:
/** C'tor that calls initialize of the concrete model implementation in order to
     *  allow for dedicated properties to be declared.
     */
    FastCaloSim(dd4hep::sim::Geant4Context* context, const std::string& nam);
  virtual ~FastCaloSim(){};

 /// Geometry construction callback. Called at "Construct()"
    virtual void constructGeo(dd4hep::sim::Geant4DetectorConstructionContext* ctxt)  override {
      this->Geant4FastSimShowerModel::constructGeo(ctxt);

    }

    /// Electromagnetic field construction callback. Called at "ConstructSDandField()"
    virtual void constructField(dd4hep::sim::Geant4DetectorConstructionContext* ctxt)  override {
      this->Geant4FastSimShowerModel::constructField(ctxt);
    }

    /// Sensitive detector construction callback. Called at "ConstructSDandField()"
    virtual void constructSensitives(dd4hep::sim::Geant4DetectorConstructionContext* ctxt)  override {
      this->Geant4FastSimShowerModel::constructSensitives(ctxt);
    }

    /// User callback to determine if the model is applicable for the particle type
    /** Default implementation checks if the particle is registered in 'ApplicableParticles'
     */
    virtual bool check_applicability(const G4ParticleDefinition& particle)  override {
      /// this model can be used with all particles
      return true;
    }

    /// User callback to determine if the shower creation should be triggered
    /** Default implementation checks if for all particles registered in 'Etrigger'
     *  the kinetic energy is bigger than the value.
     */
    virtual bool check_trigger(const G4FastTrack& track)  override;


    /// User callback to model the particle/energy shower - details defined in ML_MODEL
    virtual void modelShower(const G4FastTrack& track, G4FastStep& step) override;
};
}}

#endif /* FastCaloSim_HH */
