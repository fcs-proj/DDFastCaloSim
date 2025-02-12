######################################################################
#
#  custom steering file for simulation
#
#
######################################################################
from steering.cfg.simulation import SIM

from steering.cfg.gun import setup as setup_gun
from steering.cfg.output import setup as setup_output
from steering.cfg.physics import setup as setup_physics
from steering.cfg.random import setup as setup_random
from steering.cfg.truth import setup as setup_truth

# Apply configurations
setup_gun(SIM)
setup_output(SIM)
setup_physics(SIM)
setup_random(SIM)
setup_truth(SIM)
