######################################################################
#
#  custom steering file for ODD simulation
#
#
######################################################################
from DDSim.DD4hepSimulation import DD4hepSimulation
from g4units import mm, MeV, TeV
from math import pi

# See https://github.com/AIDASoft/DD4hep/blob/master/DDG4/python/DDSim/DD4hepSimulation.py
SIM = DD4hepSimulation()

## The compact XML file
SIM.compactFile = "OpenDataDetector.xml"
## Lorentz boost for the crossing angle, in radian!
SIM.enableDetailedShowerMode = False
SIM.enableG4GPS = False
SIM.enableG4Gun = False
SIM.enableGun = True
## InputFiles for simulation .stdhep, .slcio, .HEPEvt, .hepevt, .hepmc, .pairs files are supported
# SIM.inputFiles = [""]
## no macro file to execute for runType 'run' or 'vis'
## number of events to simulate, used in batch mode
SIM.numberOfEvents = 100

## Outputfile from the simulation,only lcio output is supported
SIM.outputFile = "output.root"

## Physics list to use in simulation
SIM.physicsList = None
## Verbosity use integers from 1(most) to 7(least) verbose
## or strings: VERBOSE, DEBUG, INFO, WARNING, ERROR, FATAL, ALWAYS
SIM.printLevel = "INFO"
## The type of action to do in this invocation
## batch: just simulate some events, needs numberOfEvents, and input file or gun
## vis: enable visualization, run the macroFile if it is set
## run: run the macroFile and exit
## shell: enable interactive session
SIM.runType = "run"  # "batch"
## Skip first N events when reading a file
SIM.skipNEvents = 0
## Steering file to change default behavior
SIM.steeringFile = None
## FourVector of translation for the Smearing of the Vertex position: x y z t
SIM.vertexOffset = [0.0, 0.0, 0.0, 0.0]
## FourVector of the Sigma for the Smearing of the Vertex position: x y z t
SIM.vertexSigma = [0.0, 0.0, 0.0, 0.0]

##  set the calorimeter action to the default scoring
SIM.action.calo = (
    "Geant4CalorimeterAction",
    {
        "HitCreationMode": 2,
    },
)

##  create a map of patterns and actions to be applied to sensitive detectors
##         example: SIM.action.mapActions['tpc'] = "TPCSDAction"
SIM.action.mapActions = {}

##  set the default tracker action
SIM.action.tracker = "Geant4VoidSensitiveAction"

##  default filter for calorimeter sensitive detectors; this is applied if no other filter is used for a calorimeter
SIM.filter.calo = "edep0"

##  a map between patterns and filter objects, using patterns to attach filters to sensitive detector
SIM.filter.mapDetFilter = {}

################################################################################
## Configuration for the GuineaPig InputFiles
################################################################################
SIM.guineapig.particlesPerEvent = "-1"


################################################################################
## Configuration for the DDG4 ParticleGun
################################################################################


##  direction of the particle gun, 3 vector
SIM.gun.direction = (0, 1, 0)

## Uniform sampling of the particle gun
SIM.gun.distribution = "uniform"

## Total energy for the particle gun.
SIM.gun.energy = 65536 * MeV

SIM.gun.multiplicity = 1
SIM.gun.particle = "gamma"

## Source position of the particle gun (TODO: generate at calorimeter surface)
SIM.gun.position = (0.0, 0.0, 0.0)

## Minimal azimuthal angle for random distribution
SIM.gun.phiMin = 0
## Maximal azimuthal angle for random distribution
SIM.gun.phiMax = 2 * pi

## Minimal pseudorapidity for random distribution (overrides thetaMax)
SIM.gun.etaMin = 0.20

## Maximal pseudorapidity for random distribution (overrides thetaMin)
SIM.gun.etaMax = 0.25

################################################################################
## Configuration for the output levels of DDG4 components
################################################################################

## Output level for input sources
SIM.output.inputStage = 3

## Output level for Geant4 kernel
SIM.output.kernel = 3

## Output level for ParticleHandler
SIM.output.part = 3

## Output level for Random Number Generator setup
SIM.output.random = 6


################################################################################
## Configuration for the Particle Handler/ MCTruth treatment
################################################################################

## Enable lots of printout on simulated hits and MC-truth information
SIM.part.enableDetailedHitsAndParticleInfo = False

##  Keep all created particles
SIM.part.keepAllParticles = False

## Minimal distance between particle vertex and endpoint of parent after
##     which the vertexIsNotEndpointOfParent flag is set
##
SIM.part.minDistToParentVertex = 2.2e-14

## MinimalKineticEnergy to store particles created in the tracking region
# This setting vastly reduces the file size as only the incident particle is stored
SIM.part.minimalKineticEnergy = 1 * TeV

##  Printout at End of Tracking
SIM.part.printEndTracking = False

##  Printout at Start of Tracking
SIM.part.printStartTracking = False

## List of processes to save, on command line give as whitespace separated string in quotation marks
SIM.part.saveProcesses = []

## Optionally enable an extended Particle Handler
SIM.part.userParticleHandler = ""

################################################################################
## Configuration for the PhysicsList
################################################################################
# this needs to be set to False if any standard physics list is used:
SIM.physics.decays = False
SIM.physics.list = "FTFP_BERT"

##  location of particle.tbl file containing extra particles and their lifetime information
# SIM.physics.pdgfile = ""

##  The global geant4 rangecut for secondary production
##
##     Default is 0.7 mm as is the case in geant4 10
##
##     To disable this plugin and be absolutely sure to use the Geant4 default range cut use "None"
##
##     Set printlevel to DEBUG to see a printout of all range cuts,
##     but this only works if range cut is not "None"
##
SIM.physics.rangecut = 0.1 * mm


################################################################################
## Properties for the random number generator
################################################################################

## If True, calculate random seed for each event based on eventID and runID
## allows reproducibility even when SkippingEvents
SIM.random.enableEventSeed = True
SIM.random.file = None
SIM.random.luxury = 1
SIM.random.replace_gRandom = True
SIM.random.seed = 42
SIM.random.type = None

###############################################################################
# Configure fast simulation
###############################################################################

# def fast_sim_config(kernel):
#    from g4units import GeV, MeV  # DO NOT REMOVE OR MOVE!!!!! (EXCLAMATION MARK)
#    from DDG4 import DetectorConstruction, Geant4, PhysicsList

#    geant4 = Geant4(kernel)
#    seq = geant4.detectorConstruction()

#    # Create a model for fast simulation
#    model = DetectorConstruction(kernel, "FastCaloSim" )

#    # Mandatory model parameters
#    model.RegionName = 'ECalBarrelRegion'
#    model.Enable = True
#    myParticles =  ["e-","e+","gamma","mu-","mu+","pi-","pi+"]
#    model.ApplicableParticles = myParticles
#    model.enableUI()
#    seq.adopt(model)
#    #-------------------
#    model_endcap = DetectorConstruction(kernel, "FastCaloSim")

#    # Mandatory model parameters
#    model_endcap.RegionName = 'ECalEndcapRegion'
#    model_endcap.Enable = True
#    model_endcap.ApplicableParticles = myParticles

#    model_endcap.enableUI()
#    seq.adopt(model_endcap)
#    #-------------------

#    # Now build the physics list:
#    phys = kernel.physicsList()
#    ph = PhysicsList(kernel, str('Geant4FastPhysics/FastPhysicsList'))
#    ph.EnabledParticles = myParticles
#    ph.BeVerbose = True
#    ph.enableUI()
#    phys.adopt(ph)
#    phys.dump()

# SIM.physics.setupUserPhysics( fast_sim_config )
