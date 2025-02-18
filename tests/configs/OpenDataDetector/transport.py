######################################################################
#
#  Steering file for DDFastCaloSim simulation (customized for testing)
#
######################################################################
import os
from steering.main import SIM
from math import pi


TEST_DIR = os.environ["TEST_DIR"]
## Geometry input file
SIM.compactFile = f"{TEST_DIR}/geometry/OpenDataDetector/OpenDataDetector.xml"
## Output file (this should be set in the ctest)
SIM.outputFile = None
## Number of events to simulate
SIM.numberOfEvents = 1

## Particle gun configuration
SIM.gun.distribution = "eta"
SIM.gun.multiplicity = 1
SIM.gun.particle = "gamma"
SIM.gun.energy = 65536
SIM.gun.etaMin = 0.20
SIM.gun.etaMax = 0.25
SIM.gun.phiMin = 0
SIM.gun.phiMax = 2 * pi

# Source position of the particle gun (TODO: generate at calorimeter surface)
SIM.gun.position = (0, 0, 0)

# SIM.gun.momentumMin = 65536
# SIM.gun.momentumMax = 65536

# Direction of the particle gun (TODO: needs to be compatible with production at origin)
# SIM.gun.direction = (0,1,0)


######################################################################
# Configure fast simulation
######################################################################


def fast_sim_cfg(kernel):
    from steering.cfg.fastsim import FastSimModelConfig

    # Import the base fast simulation model configuration
    config = FastSimModelConfig(kernel)
    # Set specific configuration
    config.region_names = ["ECalBarrelRegion", "ECalEndcapRegion"]
    config.active_particles = ["e-", "e+", "gamma", "pi-", "pi+"]
    config.transport_output_file = "transport_tracks.json"
    config.use_simplified_geo = False
    config.max_transport_steps = 1500
    # config.parametrization_pdg_id = 22
    config.transport_limit_volume = "HCalHiddenEnvelope"
    config.setup()


# Register fast simulation
SIM.physics.setupUserPhysics(fast_sim_cfg)
