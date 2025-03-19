#include "FastCaloSimConverter/HitConverter.h"

#include <TTree.h>

// EDM4hep includes
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "podio/Frame.h"
#include "podio/ROOTReader.h"

HitConverter::HitConverter(const std::string& edm4hep_file, TTree* outTree)
    : m_outTree(outTree)
{
  // Initialize the reader
  m_reader = std::make_unique<podio::ROOTReader>();
  m_reader->openFile(edm4hep_file);

  // Initialize the hit vector
  m_hits = new std::vector<FCS_hit>();

  // Create branches
  create_branches(outTree);
}

HitConverter::~HitConverter()
{
  delete m_hits;
}

void HitConverter::create_branches(TTree* outTree)
{
  if (!outTree) {
    std::cerr << "HitConverter::create_branches ERROR: outTree is null!\n";
    return;
  }

  // Create branch for the hits
  outTree->Branch("hits", &m_hits);
}

void HitConverter::process_entry(Long64_t entry)
{
  // Clear previous hits
  m_hits->clear();

  // Get the EDM4hep event - this needs to be thread-safe
  podio::Frame event;
  {
    std::lock_guard<std::mutex> lock(m_reader_mutex);
    event = podio::Frame(m_reader->readEntry("events", entry));
  }

  // Convert the hits
  convert_hits(event);
}

void HitConverter::convert_hits(const podio::Frame& event)
{
  // Get the calorimeter hits collection
  auto& schs =
      event.get<edm4hep::SimCalorimeterHitCollection>("ECalBarrelCollection");

  if (schs.isValid()) {
    // Process each hit
    for (const auto& sch : schs) {
      FCS_hit hit;

      // Fill basic properties
      hit.identifier = sch.getCellID();

      hit.cell_identifier = sch.getCellID();
      /// TODO: these layers will not be unique for the whole detector
      hit.sampling = m_decoder.get(hit.cell_identifier, "layer");
      hit.hit_energy = sch.getEnergy();

      // Fill position
      auto pos = sch.getPosition();
      hit.hit_x = pos.x;
      hit.hit_y = pos.y;
      hit.hit_z = pos.z;

      // Time from first contribution if available
      /// @TODO: is this the time we want?
      hit.hit_time = 0;
      if (sch.contributions_size() > 0) {
        auto contrib = sch.getContributions(0);
        hit.hit_time = contrib.getTime();
      }

      // Add to hits vector
      m_hits->push_back(hit);
    }
  } else {
    std::cerr << "SimCalorimeterHits collection not found or invalid\n";
  }
}
