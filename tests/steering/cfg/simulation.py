######################################################################
#  General simulation settings
######################################################################
from DDSim.DD4hepSimulation import DD4hepSimulation

SIM = DD4hepSimulation()

## The compact XML file
SIM.compactFile = "OpenDataDetector.xml"
## Lorentz boost for the crossing angle, in radian!
SIM.enableDetailedShowerMode = False
# Disable particle gun as we are using HepMC input
SIM.enableG4GPS = False
SIM.enableG4Gun = False
SIM.enableGun = False
## number of events to simulate, used in batch mode
SIM.numberOfEvents = -1
## Output file from the simulation
SIM.outputFile = "output.root"
## Physics list to use in simulation
SIM.physicsList = None
## Verbosity level
SIM.printLevel = "INFO"
## The type of action to do in this invocation
SIM.runType = "run"  # Options: "batch", "vis", "shell"
## Skip first N events when reading a file
SIM.skipNEvents = 0
## Steering file to change default behavior
SIM.steeringFile = None
## FourVector of translation for the Smearing of the Vertex position: x y z t
SIM.vertexOffset = [0.0, 0.0, 0.0, 0.0]
## FourVector of the Sigma for the Smearing of the Vertex position: x y z t
SIM.vertexSigma = [0.0, 0.0, 0.0, 0.0]
