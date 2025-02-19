######################################################################
#
#  Steering file for DDFastCaloSim simulation (customized for testing)
#
######################################################################
import os
import numpy as np
from steering.main import SIM
from python.ParticleEventGenerator import ParticleEventGenerator


# Base and working directories
TEST_BASE_DIR = os.getenv("TEST_BASE_DIR")
TEST_WORKING_DIR = os.getenv("TEST_WORKING_DIR")

## Geometry input file
SIM.compactFile = f"{TEST_BASE_DIR}/geometry/OpenDataDetector/OpenDataDetector.xml"

# Configure the number of events to simulate
SIM.numberOfEvents = 5

######################################################################
# Configure particle generation
######################################################################

# Generate single geantino events
# uniform sampling in 0.20 < eta < 0.25
# uniform sampling in 0 < phi < 2*pi
# truth momentum = 100 MeV
gen = ParticleEventGenerator(seed=42)
gen.generate(
    nEvents=SIM.numberOfEvents,
    pid=0,
    momentum=100,
    mass=0,
    phi_min=0,
    phi_max=2 * np.pi,
    eta_min=0.20,
    eta_max=0.25,
)

evnt_path = os.path.join(TEST_WORKING_DIR, "event.hepmc")
gen.save(evnt_path)

SIM.inputFiles = [evnt_path]

######################################################################
# Configure fast simulation
######################################################################


def fast_sim_cfg(kernel):
    from steering.cfg.fastsim import FastSimModelConfig

    # Import the base fast simulation model configuration
    config = FastSimModelConfig(kernel)
    # Set the fast simulation model name
    config.model_name = "ParametrizationModel"
    # Set specific configuration
    config.region_name = "ECalBarrelRegion"
    # Only geantinos will trigger the fast simulation
    config.active_particles = ["geantino"]
    # Use simplified geometry for calorimeter transport?
    config.use_simplified_geo = False
    # Maximum number of transport steps for calorimeter transport
    config.max_transport_steps = 1500
    # Maximum volume to which transport is performed
    config.transport_limit_volume = "NotImplementedYet"
    # PDG ID of the particle to be parametrized
    config.parametrization_pdg_id = 22

    # Setup the fast simulation model
    config.setup()


# Register fast simulation
SIM.physics.setupUserPhysics(fast_sim_cfg)
