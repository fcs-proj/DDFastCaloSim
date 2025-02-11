######################################################################
## Configuration for the DDG4 ParticleGun
######################################################################
from g4units import MeV
from math import pi


def setup(SIM):
    # Direction of the particle gun
    SIM.gun.direction = (0, 1, 0)

    # Uniform sampling of the particle gun
    SIM.gun.distribution = "uniform"

    # Total energy for the particle gun
    SIM.gun.energy = 65536 * MeV

    # Multiplicity of the particle gun
    SIM.gun.multiplicity = 1

    # Particle type for the particle gun
    SIM.gun.particle = "gamma"

    # Source position of the particle gun (TODO: generate at calorimeter surface)
    SIM.gun.position = (0.0, 0.0, 0.0)

    # Minimal azimuthal angle for random distribution
    SIM.gun.phiMin = 0

    # Maximal azimuthal angle for random distribution
    SIM.gun.phiMax = 2 * pi

    # Minimal pseudorapidity for random distribution (overrides thetaMax)
    SIM.gun.etaMin = 0.20

    # Maximal pseudorapidity for random distribution (overrides thetaMin)
    SIM.gun.etaMax = 0.25
