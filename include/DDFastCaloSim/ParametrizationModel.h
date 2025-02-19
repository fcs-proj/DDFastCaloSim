#ifndef ParametrizationModel_HH
#define ParametrizationModel_HH

#include <DDFastCaloSim/FastCaloSimModel.h>

class G4FastStep;
class G4FastTrack;

namespace dd4hep::sim
{
class ParametrizationModel : public dd4hep::sim::FastCaloSimModel
{
public:
  /// Standard constructor
  ParametrizationModel(dd4hep::sim::Geant4Context* context,
                       const std::string& name);

  // Default destructor
  ~ParametrizationModel() override
  {
    // Serialize the tracks
    if (m_transportTracks.size() > 0 && !m_transport_output.empty()) {
      try {
        m_transportTracks.serialize(m_transport_output);
      } catch (const std::exception& e) {
        std::cerr << "Failed to serialize transport tracks: " << e.what()
                  << std::endl;
      }
    }
  };

  /// User callback to implement the parametrization model
  void modelShower(const G4FastTrack& track, G4FastStep& step) override;
};
}  // namespace dd4hep::sim

#endif /* ParametrizationModel_HH */
