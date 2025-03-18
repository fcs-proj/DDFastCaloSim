#pragma once

#include <TFile.h>
#include <TTree.h>

class InputLoader
{
public:
  InputLoader() = default;

  ~InputLoader()
  {
    for (auto& [_, file] : open_files_) {
      if (file) {
        file->Close();
        delete file;
      }
    }
  }

  /**
   * Get a tree from a file path with specified tree name
   * @param file_path Path to the ROOT file
   * @param tree_name Name of the tree to retrieve
   * @return Pointer to the tree, or nullptr if not found
   */
  auto get_tree(const std::string& file_path, const std::string& tree_name)
      -> TTree*
  {
    // Check if we already have this file open
    TFile* file = nullptr;
    if (open_files_.find(file_path) != open_files_.end()) {
      file = open_files_[file_path];
    } else {
      // Open the file
      file = TFile::Open(file_path.c_str(), "READ");
      if (!file || file->IsZombie()) {
        std::cerr << "Error: Failed to open file: " << file_path << std::endl;
        return nullptr;
      }

      // Store the file in our map
      open_files_[file_path] = file;
    }

    // Get the tree
    TTree* tree = dynamic_cast<TTree*>(file->Get(tree_name.c_str()));
    if (!tree) {
      std::cerr << "Error: Failed to find TTree '" << tree_name
                << "' in file: " << file_path << std::endl;
      return nullptr;
    }

    // Check if the tree has entries
    if (tree->GetEntries() == 0) {
      std::cerr << "Warning: Tree '" << tree_name << "' has no entries"
                << std::endl;
    }

    return tree;
  }

private:
  // Map of file paths to open TFile pointers
  std::map<std::string, TFile*> open_files_;
};
