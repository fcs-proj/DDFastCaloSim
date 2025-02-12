######################################################################
#
#  Steering file for DDFastCaloSim simulation (customized for testing)
#
######################################################################
import os
from steering.main import SIM


TEST_DIR = os.environ["TEST_DIR"]
## Geometry input file
SIM.compactFile = f"{TEST_DIR}/geometry/OpenDataDetector/OpenDataDetector.xml"
## Output file
SIM.outputFile = f"{TEST_DIR}/output/ODD_sim.root"
## Number of events to simulate
SIM.numberOfEvents = 1

## Particle gun configuration
SIM.gun.particle = "gamma"
SIM.gun.energy = 65536
SIM.gun.etaMin = 0.20
SIM.gun.etaMax = 0.25
SIM.gun.phiMin = 0
SIM.gun.phiMax = 6.28318530718  # 2 * pi


######################################################################
# Configure fast simulation
######################################################################


def setup(kernel):
    from DDG4 import DetectorConstruction, Geant4, PhysicsList

    geant4 = Geant4(kernel)
    seq = geant4.detectorConstruction()

    # Mandatory model parameters
    region_names = ["ECalBarrelRegion", "ECalEndcapRegion"]
    active_particles = ["e-", "e+", "gamma", "pi-", "pi+"]

    for region in region_names:
        # Create model
        model = DetectorConstruction(kernel, "FastCaloSimModel")
        # Attach model to region
        model.RegionName = region
        # Enable model
        model.Enable = True
        # Set active particles
        model.ApplicableParticles = active_particles
        model.enableUI()
        seq.adopt(model)

    # Get the physics list
    physics_list = kernel.physicsList()
    # Define the fast simulation physics list
    fast_physics_list = PhysicsList(kernel, str("Geant4FastPhysics/FastPhysicsList"))
    fast_physics_list.EnabledParticles = active_particles
    fast_physics_list.BeVerbose = True
    fast_physics_list.enableUI()
    # Adopt the fast simulation physics list
    physics_list.adopt(fast_physics_list)
    physics_list.dump()


# Register fast simulation
SIM.physics.setupUserPhysics(setup)
