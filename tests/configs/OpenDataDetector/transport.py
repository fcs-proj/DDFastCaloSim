"""
######################################################################
#  Steering file for DDFastCaloSim simulation (customized for testing)
######################################################################
"""

import os
import numpy as np
from steering.main import SIM
from python.ParticleEventGenerator import ParticleEventGenerator

# Base and working directories
TEST_BASE_DIR = os.getenv("TEST_BASE_DIR")
TEST_WORKING_DIR = os.getenv("TEST_WORKING_DIR")

# Simulation configuration
SIM.compactFile = f"{TEST_BASE_DIR}/geometry/OpenDataDetector/OpenDataDetector.xml"

# Configure the number of events to simulate
SIM.numberOfEvents = 1

######################################################################
# Configure particle generation
######################################################################

# Generate single event with multiple geantinos
# Eta of geantinos -1.5, -1.4, ..., 1.4, 1.5
eta_list = np.arange(-1.5, 1.5, 0.1)

gen = ParticleEventGenerator(seed=42)
gen.generate(
    nEvents=SIM.numberOfEvents,
    pid=0,
    momentum=10.0,
    mass=0,
    phi=0,
    eta_list=eta_list,
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
    # PDG ID of the particle to be transported
    config.parametrization_pdg_id = 22
    # Output file for transport tracks
    config.transport_output_file = "transport_tracks.json"

    # Setup the fast simulation model
    config.setup()


# Register fast simulation
SIM.physics.setupUserPhysics(fast_sim_cfg)
