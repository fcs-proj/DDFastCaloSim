
#include "DDFastCaloSim/ParametrizationWriter.h"

#include <DDG4/Factories.h>
#include <Parsers/Printout.h>

#include <G4Event.hh>
#include <G4Run.hh>

using namespace dd4hep::sim;

/// -------------------------------------------------------------------
/// Geant4 EVENT ACTION to collect per-event data
/// -------------------------------------------------------------------

ParamCollector::ParamCollector(Geant4Context* context, const std::string& name)
    : Geant4EventAction(context, name)
{
}

ParamCollector::~ParamCollector() {}

void ParamCollector::begin(const G4Event* /*event*/)
{
  // Clear event data buffer at the beginning of each event
  event_data.clear();
}

void ParamCollector::end(const G4Event* /*event*/)
{
  // Write the event data to the ROOT file at the end of each event
  auto writer =
      dynamic_cast<ParamWriter*>(context()->runAction().get("ParamWriter"));
  if (writer) {
    writer->write_event(event_data);
  } else {
    dd4hep::printout(dd4hep::ERROR,
                     "ParamCollector",
                     "ParamWriter not found in run action sequence!");
  }
}

/// -------------------------------------------------------------------
/// Geant4 RUN ACTION to write data to ROOT file
/// -------------------------------------------------------------------

ParamWriter::ParamWriter(Geant4Context* context, const std::string& name)
    : Geant4RunAction(context, name)
{
}

ParamWriter::~ParamWriter()
{
  delete output_file;
}

void ParamWriter::begin(const G4Run* /*run*/)
{
  output_file = new TFile(m_param_output_file.c_str(), "RECREATE");
  tree = new TTree(m_param_tree_name.c_str(), "Input for parametrization");
  event_data.bind_to_tree(tree);
}

void ParamWriter::end(const G4Run* /*run*/)
{
  if (!output_file) {
    dd4hep::printout(dd4hep::ERROR, "ParamWriter", "Output file not found!");
    return;
  }

  if (output_file->IsZombie()) {
    dd4hep::printout(dd4hep::ERROR, "ParamWriter", "Output file is a zombie!");
    return;
  }

  output_file->Write();
  output_file->Close();
}

/// -------------------------------------------------------------------
/// Writes a single event's data to the ROOT file
/// -------------------------------------------------------------------
void ParamWriter::write_event(const EventData& data)
{
  if (!tree) {
    dd4hep::printout(dd4hep::ERROR, "ParamWriter", "Tree not found!");
    return;
  }
  // Copy the event data to the tree
  event_data = data;
  tree->Fill();
}

DECLARE_GEANT4ACTION(ParamCollector)
DECLARE_GEANT4ACTION(ParamWriter)
