######################################################################
# Configure fast simulation
######################################################################
from DDG4 import DetectorConstruction, Geant4, PhysicsList


def setup(kernel):
    geant4 = Geant4(kernel)
    seq = geant4.detectorConstruction()

    # Create a model for fast simulation
    model = DetectorConstruction(kernel, "FastCaloSimModel")

    # Mandatory model parameters
    model.RegionName = "ECalBarrelRegion"
    model.Enable = True
    myParticles = ["e-", "e+", "gamma", "pi-", "pi+"]
    model.ApplicableParticles = myParticles
    model.enableUI()
    seq.adopt(model)
    # -------------------
    model_endcap = DetectorConstruction(kernel, "FastCaloSimModel")

    # Mandatory model parameters
    model_endcap.RegionName = "ECalEndcapRegion"
    model_endcap.Enable = True
    model_endcap.ApplicableParticles = myParticles

    model_endcap.enableUI()
    seq.adopt(model_endcap)
    # -------------------

    # Now build the physics list:
    phys = kernel.physicsList()
    ph = PhysicsList(kernel, str("Geant4FastPhysics/FastPhysicsList"))
    ph.EnabledParticles = myParticles
    ph.BeVerbose = True
    ph.enableUI()
    phys.adopt(ph)
    phys.dump()
