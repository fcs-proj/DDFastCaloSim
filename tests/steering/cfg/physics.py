######################################################################
#  Configuration for the Physics List
######################################################################
from g4units import mm


def setup(SIM):
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
