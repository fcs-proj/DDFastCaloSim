#pragma once

#include "DDFastCaloSim/ParamOutputEDM.h"
#include "DDG4/Geant4EventAction.h"
#include "DDG4/Geant4RunAction.h"
#include "TFile.h"
#include "TTree.h"

namespace dd4hep::sim
{

/// -------------------------------------------------------------------
/// Geant4 EVENT ACTION to collect per-event data
/// -------------------------------------------------------------------

class ParamCollector : public Geant4EventAction
{
public:
  ParamCollector(Geant4Context* context, const std::string& name);
  virtual ~ParamCollector();

  /// @brief Called at the beginning of each event
  virtual void begin(const G4Event* event) override;
  /// @brief Called at the end of each event
  virtual void end(const G4Event* event) override;

  /// @brief Public accessor to add data to the event data buffer
  auto get_event_data() -> EventData& { return event_data; }

private:
  /// @brief Buffer to store per-event data
  EventData event_data;
};

/// -------------------------------------------------------------------
/// Geant4 RUN ACTION to write data to ROOT file
/// -------------------------------------------------------------------

class ParamWriter : public Geant4RunAction
{
public:
  ParamWriter(Geant4Context* context, const std::string& name);
  virtual ~ParamWriter();

  /// @brief Called at the beginning of the run
  virtual void begin(const G4Run* run) override;
  /// @brief Called at the end of the run
  virtual void end(const G4Run* run) override;

  /// Writes a single event to the ROOT file (called by `ParamCollector`)
  void write_event(const EventData& data);

  /// @brief Output setters
  void set_param_output_file(const std::string& param_output_file)
  {
    m_param_output_file = param_output_file;
  }
  void set_param_tree_name(const std::string& param_tree_name)
  {
    m_param_tree_name = param_tree_name;
  }

private:
  TFile* output_file {nullptr};
  TTree* tree {nullptr};

  // Event level buffer to store single event
  EventData event_data;

  /// @brief Name of the ROOT file to write param information to
  std::string m_param_output_file = "param_output.root";
  /// @brief Name of the TTree to write param information to
  std::string m_param_tree_name = "param_info";
};

}  // namespace dd4hep::sim
