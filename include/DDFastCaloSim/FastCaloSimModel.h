#ifndef FastCaloSimModel_HH
#define FastCaloSimModel_HH

#include <DDG4/Geant4FastSimShowerModel.h>

// -- FastCaloSim includes
#include "FastCaloSim/Core/TFCSParametrizationBase.h"
#include "FastCaloSim/Extrapolation/FastCaloSimCaloExtrapolation.h"
#include "FastCaloSim/Transport/G4CaloTransportTool.h"

// -- Test helper includes
#include "TestHelpers/TrackContainer.h"

class G4FastStep;
class G4FastTrack;
class G4ParticleDefinition;
class G4Region;

/**
 * @brief Register particles at calorimeter layer
 *
 */

namespace dd4hep::sim
{
class FastCaloSimModel : public dd4hep::sim::Geant4FastSimShowerModel
{
public:
  /// Standard constructor
  FastCaloSimModel(dd4hep::sim::Geant4Context* context,
                   const std::string& name);

  // Default destructor
  ~FastCaloSimModel() override
  {
    // Serialize the tracks
    if (m_transportTracks.size() > 0 && !m_transport_output.empty()) {
      m_transportTracks.serialize(m_transport_output);
    }
  };

  /// Geometry construction callback. Called at Construct()
  void constructGeo(
      dd4hep::sim::Geant4DetectorConstructionContext* ctxt) override
  {
    this->Geant4FastSimShowerModel::constructGeo(ctxt);
  }

  /// Electromagnetic field construction callback
  // Called at ConstructSDandField()
  void constructField(
      dd4hep::sim::Geant4DetectorConstructionContext* ctxt) override
  {
    this->Geant4FastSimShowerModel::constructField(ctxt);
  }

  /// Sensitive detector construction callback
  // Called at ConstructSDandField()
  void constructSensitives(
      dd4hep::sim::Geant4DetectorConstructionContext* ctxt) override
  {
    this->Geant4FastSimShowerModel::constructSensitives(ctxt);
  }

  /// User callback to determine if the model is applicable for the particle
  auto check_applicability(const G4ParticleDefinition& particle)
      -> bool override
  {
    /// this model can be used with all particles
    return true;
  }

  /// User callback to determine if the shower creation should be triggered
  auto check_trigger(const G4FastTrack& track) -> bool override;

  /// User callback to implement the fast simulation model
  void modelShower(const G4FastTrack& track, G4FastStep& step) override;

private:
  // Core FastCaloSim API
  TFCSParametrizationBase m_parametrization;
  // Geant4 transport tool
  G4CaloTransportTool m_transportTool;
  // Extrapolation tool
  FastCaloSimCaloExtrapolation m_extrapolationTool;
  // Flag to see if the transport tool is initialized
  bool m_transport_init;

  // --Test helper settings

  // Vector of transported tracks
  TestHelpers::TrackContainer m_transportTracks;

  /// @brief Flag to use simplified geometry for transport
  bool m_use_simplified_geo;
  /// @brief Name of the volume the transport is limited to
  std::string m_transport_limit_volume;
  /// @brief Maximum number of transport steps
  int m_max_transport_steps;
  /// @brief (Optional) Name of the file to serialize the transport tracks to
  std::string m_transport_output;
};
}  // namespace dd4hep::sim

#endif /* FastCaloSimModel_HH */
