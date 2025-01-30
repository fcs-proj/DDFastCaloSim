#ifndef FastCaloSimModel_HH
#define FastCaloSimModel_HH

#include <DDG4/Geant4FastSimShowerModel.h>

// -- FastCaloSim includes
#include "FastCaloSim/Core/TFCSParametrizationBase.h"
#include "FastCaloSim/Extrapolation/FastCaloSimCaloExtrapolation.h"
#include "FastCaloSim/Transport/G4CaloTransportTool.h"

class G4FastStep;
class G4FastTrack;
class G4ParticleDefinition;
class G4Region;

/**
 * @brief Register particles at calorimeter layer
 *
 */
namespace dd4hep
{
namespace sim
{
class FastCaloSimModel : public dd4hep::sim::Geant4FastSimShowerModel
{
public:
  /// Standard constructor
  FastCaloSimModel(dd4hep::sim::Geant4Context* context,
                   const std::string& name);
  virtual ~FastCaloSimModel() {};

  /// Geometry construction callback. Called at Construct()
  virtual void constructGeo(
      dd4hep::sim::Geant4DetectorConstructionContext* ctxt) override
  {
    this->Geant4FastSimShowerModel::constructGeo(ctxt);
  }

  /// Electromagnetic field construction callback
  // Called at ConstructSDandField()
  virtual void constructField(
      dd4hep::sim::Geant4DetectorConstructionContext* ctxt) override
  {
    this->Geant4FastSimShowerModel::constructField(ctxt);
  }

  /// Sensitive detector construction callback
  // Called at ConstructSDandField()
  virtual void constructSensitives(
      dd4hep::sim::Geant4DetectorConstructionContext* ctxt) override
  {
    this->Geant4FastSimShowerModel::constructSensitives(ctxt);
  }

  /// User callback to determine if the model is applicable for the particle
  virtual bool check_applicability(
      const G4ParticleDefinition& particle) override
  {
    /// this model can be used with all particles
    return true;
  }

  /// User callback to determine if the shower creation should be triggered
  virtual bool check_trigger(const G4FastTrack& track) override;

  /// User callback to implement the fast simulation model
  virtual void modelShower(const G4FastTrack& track, G4FastStep& step) override;

private:
  // Core FastCaloSim API
  TFCSParametrizationBase m_parametrization;
  // Geant4 transport tool
  G4CaloTransportTool m_transportTool;
  // Extrapolation tool
  FastCaloSimCaloExtrapolation m_extrapolationTool;
};
}  // namespace sim
}  // namespace dd4hep

#endif /* FastCaloSimModel_HH */
