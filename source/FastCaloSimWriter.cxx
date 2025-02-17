#include "DDFastCaloSim/FastCaloSimWriter.h"

#include <DD4hep/Printout.h>
#include <DDG4/Factories.h>
#include <DDG4/Geant4Context.h>

#include <G4Event.hh>

dd4hep::sim::FastCaloSimWriter::FastCaloSimWriter(
    dd4hep::sim::Geant4Context* context, const std::string& name)
    : Geant4EventAction(context, name)
{
  dd4hep::printout(dd4hep::ALWAYS,
                   "FastCaloSimWriter",
                   "Constructor: FastCaloSimWriter created");
}

void dd4hep::sim::FastCaloSimWriter::begin(const G4Event* /*event*/)
{
  // Clear the container at the beginning of each event.
  print("Calling the writer begin method");
  m_dataVec.clear();
}

void dd4hep::sim::FastCaloSimWriter::end(const G4Event* /*event*/) {}

void dd4hep::sim::FastCaloSimWriter::addData(float value)
{
  // For each track, store the float as a one-element vector.
  std::cout << "Calling the writer addData method" << std::endl;
  std::cout << "value: " << value << std::endl;
  m_dataVec.push_back({value});
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(FastCaloSimWriter)
