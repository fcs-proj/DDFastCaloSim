#pragma once

#include <string>
#include <gtest/gtest.h>

namespace TestHelpers::IOManager
{
inline auto test_suite_name() -> std::string
{
  return ::testing::UnitTest::GetInstance()
      ->current_test_info()
      ->test_suite_name();
}

inline auto test_name() -> std::string
{
  return ::testing::UnitTest::GetInstance()->current_test_info()->name();
}

inline auto create_test_output_dir() -> std::string
{
  const std::string output_dir =
      TEST_OUTPUT_DIR + test_suite_name() + "_" + test_name() + "/";
  // Create the output directory if it does not exist
  const std::string command = "mkdir -p " + output_dir;
  system(command.c_str());

  return output_dir;
}

inline auto get_test_output_dir(const std::string& test_suite_name,
                                const std::string& test_name)
{
  const std::string output_dir =
      TEST_OUTPUT_DIR + test_suite_name + "_" + test_name + "/";
  return output_dir;
}

inline auto ref_dir() -> std::string
{
  return TEST_REFS_DIR + test_suite_name() + "_" + test_name() + "/";
}

inline void preload_ddfastcalosim()
{
  const char* lib_path = std::getenv("DDFastCaloSim_LIB");
  if (!lib_path) {
    FAIL() << "DDFastCaloSim_LIB environment variable is not set";
  }
  setenv("LD_PRELOAD", lib_path, 1);
}

}  // namespace TestHelpers::IOManager
