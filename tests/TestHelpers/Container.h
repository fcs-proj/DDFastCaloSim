// Copyright (c) 2024 CERN for the benefit of the FastCaloSim project

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace TestHelpers
{

template<typename ElementType, typename SerializedType>
class Container
{
public:
  Container() = default;

  explicit Container(std::vector<ElementType> container)
      : m_container(std::move(container))
  {
  }

  /**
   * @brief Add an element to the container
   */
  void add(const ElementType& element) { m_container.push_back(element); }

  /**
   * @brief Clear the container
   */
  void clear() { m_container.clear(); }

  /**
   * @brief Access element at index
   */
  auto at(size_t index) const -> const ElementType&
  {
    return m_container.at(index);
  }
  /**
   * @brief Access size of container
   */
  auto size() const -> std::size_t { return m_container.size(); }

  /**
   * @brief Retrieve the container
   */
  auto get() const -> const std::vector<ElementType>& { return m_container; }

  /**
   * @brief Serialize the container to a JSON file
   */
  void serialize(const std::string& filename) const
  {
    try {
      nlohmann::json j = flatten();
      if (j.empty()) {
        throw std::runtime_error("JSON serialization failed. Object is empty!");
      }
      std::ofstream ofs(filename);
      if (!ofs) {
        throw std::runtime_error("Failed to open output file: " + filename);
      }
      ofs << j.dump();
      if (!ofs) {
        throw std::runtime_error("Failed to write to output file: " + filename);
      }
      ofs.close();
    } catch (const nlohmann::json::exception& e) {
      throw std::runtime_error("JSON error: " + std::string(e.what()));
    }
  }

protected:
  /**
   * @brief Virtual method which needs to implement the flattening of the
   * container to a vector of SerializedType
   */
  virtual auto flatten() const -> std::vector<SerializedType> = 0;

  std::vector<ElementType> m_container;
};

}  // namespace TestHelpers
