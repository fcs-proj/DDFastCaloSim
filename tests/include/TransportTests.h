#pragma once

#include <gtest/gtest.h>

#include "TestConfig/TransportTestsConfig.h"
#include "TestHelpers/IOManager.h"

class TransportTests : public ::testing::Test
{
protected:
  static void SetUpTestSuite()
  {
    /// -------------------------------------------------------------------
    /// Pre-load the DDFastCaloSim library
    /// -------------------------------------------------------------------

    /// TODO: instead of this we should be able to just source
    /// thisDDFastCaloSim.sh however something in key4hep is not working
    /// properly
    TestHelpers::IOManager::preload_ddfastcalosim();
  }
};
