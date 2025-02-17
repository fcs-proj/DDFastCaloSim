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

        # ----- User properties of the model ----------

        # If set, transport tracks will be saved to the output file
        model.TransportOutputFile = "transport_tracks.json"
        # Whether to use the simplified geometry for transport
        model.UseSimplifiedGeo = False
        # Maximum number of steps for transport
        model.MaxTransportSteps = 1500
        # Volume name for transport limit
        model.TransportLimitVolume = "HCalHiddenEnvelope"

        seq.adopt(model)

    # Get the physics lists
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
