#pragma once

#include <string>
#include <vector>

#include <DDG4/Geant4Context.h>
#include <DDG4/Geant4EventAction.h>

namespace dd4hep::sim
{

/**
 * @brief Custom event extension to store a vector-of-vectors of floats.
 *
 * This object is attached to the DD4hep event so that the output file contains
 * a branch with your data.
 */
class MyEventData
{
public:
  std::vector<std::vector<float>> myVector;
};

/**
 * @brief Aggregator (writer) that collects a single float per track into a
 * vector-of-vectors.
 *
 * This class derives from DDG4’s Geant4EventAction so that its begin/end
 * methods are called at the beginning and end of each event.
 */
class FastCaloSimWriter : public Geant4EventAction
{
public:
  /// Constructor: note that the context is passed as a pointer.
  FastCaloSimWriter(dd4hep::sim::Geant4Context* context,
                    const std::string& name);

  /// Called at the beginning of each event.
  virtual void begin(const G4Event* event) override;

  /// Called at the end of each event.
  virtual void end(const G4Event* event) override;

  /// Add one float (from one track) to the container (stored as a one-element
  /// vector).
  void addData(float value);

private:
  std::vector<std::vector<float>> m_dataVec;
};

}  // namespace dd4hep::sim
