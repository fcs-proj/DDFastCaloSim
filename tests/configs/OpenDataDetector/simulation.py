######################################################################
#
#  Steering file for DDFastCaloSim simulation (customized for testing)
#
######################################################################
import os
from steering.main import SIM  # Import the base configuration

######################################################################
## Override Simulation Parameters for This Test
######################################################################

TEST_DIR = os.environ["TEST_DIR"]

## Geometry input file
SIM.compactFile = f"{TEST_DIR}/geometry/OpenDataDetector/OpenDataDetector.xml"

## Number of events to simulate
SIM.numberOfEvents = 1

## Particle gun configuration
SIM.gun.particle = "gamma"
SIM.gun.energy = 65536
SIM.gun.etaMin = 0.20
SIM.gun.etaMax = 0.25
SIM.gun.phiMin = 0
SIM.gun.phiMax = 6.28318530718  # 2 * pi

## Output file configuration
SIM.outputFile = f"{TEST_DIR}/output/ODD_sim.root"
