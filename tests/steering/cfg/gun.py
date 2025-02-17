######################################################################
## Configuration for the DDG4 ParticleGun
######################################################################


def setup(SIM):
    ## Options for random distributions:
    ##  'uniform' is the default distribution, flat in theta
    ##  'cos(theta)' is flat in cos(theta)
    ##  'eta', or 'pseudorapidity' is flat in pseudorapity
    ##  'ffbar' is distributed according to 1+cos^2(theta)
    SIM.gun.distribution = "eta"

    # Multiplicity of the particle gun
    SIM.gun.multiplicity = 1

    # Total energy for the particle gun
    # SIM.gun.energy = None

    # Particle type for the particle gun
    # SIM.gun.particle = None

    # Source position of the particle gun
    # SIM.gun.position = None

    # Direction of the particle gun
    # SIM.gun.direction = None

    # Minimal azimuthal angle for random distribution
    # SIM.gun.phiMin = None

    # Maximal azimuthal angle for random distribution
    # SIM.gun.phiMax = None

    # Minimal pseudorapidity for random distribution (overrides thetaMax)
    # SIM.gun.etaMin = None

    # Maximal pseudorapidity for random distribution (overrides thetaMin)
    # SIM.gun.etaMax = None
