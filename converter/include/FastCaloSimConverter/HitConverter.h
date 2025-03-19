#pragma once
#include <memory>
#include <mutex>

#include <DDSegmentation/BitFieldCoder.h>
#include <RtypesCore.h>

#include "FastCaloSimParam/FCS_Cell.h"

class TTree;
class TTreeReader;

// Forward declarations for EDM4hep
namespace podio
{
class Frame;
class ROOTReader;
}  // namespace podio

class HitConverter
{
public:
  /// @brief Creates a converter for EDM4hep hits
  HitConverter(const std::string& edm4hep_file, TTree* outTree);

  ~HitConverter();

  /// @brief Processes an entry and fills hit data
  void process_entry(Long64_t entry);

private:
  /// @brief Creates branches in the output tree
  void create_branches(TTree* outTree);

  /// @brief Convert EDM4hep hits to FCS hits
  void convert_hits(const podio::Frame& event);

  // EDM4hep reader (protected by mutex for MT access)
  std::unique_ptr<podio::ROOTReader> m_reader;
  std::mutex m_reader_mutex;

  // Output data container
  std::vector<FCS_hit>* m_hits = nullptr;

  // Output tree
  TTree* m_outTree = nullptr;

  // Bitfield decoder to access detector information (e.g. layer number)
  /// TODO: Can we directly access schema information from file?
  dd4hep::DDSegmentation::BitFieldCoder m_decoder {
      "system:8,barrel:3,module:4,layer:6,slice:5,x:32:-16,y:-16"};
};
