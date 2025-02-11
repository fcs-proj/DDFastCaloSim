######################################################################
#  Configuration for output levels of DDG4 components
######################################################################
def setup(SIM):
    ## Output level for input sources
    SIM.output.inputStage = 3
    ## Output level for Geant4 kernel
    SIM.output.kernel = 3
    ## Output level for ParticleHandler
    SIM.output.part = 3
    ## Output level for Random Number Generator setup
    SIM.output.random = 6
