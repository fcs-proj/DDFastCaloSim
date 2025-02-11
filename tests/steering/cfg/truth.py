######################################################################
## Configuration for the MCTruth treatment
######################################################################
from g4units import TeV


def setup(SIM):
    # Enable lots of printout on simulated hits and MC-truth information
    SIM.part.enableDetailedHitsAndParticleInfo = False

    # Keep all created particles
    SIM.part.keepAllParticles = False

    # Minimal distance between particle vertex and endpoint of parent after
    # which the vertexIsNotEndpointOfParent flag is set
    SIM.part.minDistToParentVertex = 2.2e-14

    # MinimalKineticEnergy to store particles created in the tracking region
    # This setting vastly reduces the file size as only the incident particle is stored
    SIM.part.minimalKineticEnergy = 1 * TeV

    # Printout at End of Tracking
    SIM.part.printEndTracking = False

    # Printout at Start of Tracking
    SIM.part.printStartTracking = False

    # List of processes to save, on command line give as whitespace separated string in quotation marks
    SIM.part.saveProcesses = []

    # Optionally enable an extended Particle Handler
    SIM.part.userParticleHandler = ""
