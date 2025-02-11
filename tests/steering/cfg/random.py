######################################################################
#  Properties for the random number generator
######################################################################
def setup(SIM):
    # If True, calculate random seed for each event based on eventID and runID
    # allows reproducibility even when SkippingEvents
    SIM.random.enableEventSeed = True
    SIM.random.file = None
    SIM.random.luxury = 1
    SIM.random.replace_gRandom = True
    SIM.random.seed = 42
    SIM.random.type = None
