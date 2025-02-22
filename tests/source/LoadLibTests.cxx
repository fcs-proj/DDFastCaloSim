#include <TInterpreter.h>
#include <TSystem.h>
#include <gtest/gtest.h>

TEST(LoadLibTests, LoadFastCaloSimClasses)
{
  // Get the path to the DDFastCaloSim library (libDDFastCaloSim.so)
  const char* ddFCSLib = std::getenv("DDFastCaloSim_LIB");

  ASSERT_NE(ddFCSLib, nullptr)
      << "Environment variable DDFastCaloSimLib is not set!";

  // Load the library with ROOT's gSystem
  int loadStatus = gSystem->Load(ddFCSLib);
  if (loadStatus != 0) {
    std::cerr << "gSystem->Load() failed: " << gSystem->GetErrorStr()
              << std::endl;
  }

  // Check if the library was loaded successfully
  ASSERT_EQ(loadStatus, 0) << "Failed to load the DDFastCaloSim library.";

  // Try to create instances of classes from the FastCaloSim library
  auto* truthState =
      reinterpret_cast<void*>(gInterpreter->ProcessLine("new TFCSTruthState;"));

  auto* extrapolState = reinterpret_cast<void*>(
      gInterpreter->ProcessLine("new TFCSExtrapolationState;"));

  auto* simulState = reinterpret_cast<void*>(
      gInterpreter->ProcessLine("new TFCSSimulationState;"));

  // Check if the object was successfully created
  ASSERT_NE(truthState, nullptr) << "Failed to instantiate TFCSTruthState.";
  ASSERT_NE(extrapolState, nullptr)
      << "Failed to instantiate TFCSExtrapolationState.";
  ASSERT_NE(simulState, nullptr)
      << "Failed to instantiate TFCSSimulationState.";
}
