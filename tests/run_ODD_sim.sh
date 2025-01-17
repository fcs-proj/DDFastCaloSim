#!/bin/bash

# Simulation Parameters
NUMEVENTS=1
ENERGY=65536
PARTICLE="gamma"
ETA_MIN=0.20
ETA_MAX=0.25
PHI_MIN=0
PHI_MAX=6.28318530718 # 2 * pi

STEERING_FILE=$(realpath ../steering/OpenDataDetector.py)
GEOMETRY_FILE=$(realpath ../geometry/OpenDataDetector/OpenDataDetector.xml)

# Output directory
mkdir -p run

# Run the simulation
(
cd run || exit
ddsim \
  --steeringFile ${STEERING_FILE} \
  --compactFile ${GEOMETRY_FILE} \
  --numberOfEvents ${NUMEVENTS} \
  --gun.particle ${PARTICLE} \
  --gun.etaMin ${ETA_MIN} \
  --gun.etaMax ${ETA_MAX} \
  --gun.phiMin ${PHI_MIN} \
  --gun.phiMax ${PHI_MAX} \
  --gun.energy ${ENERGY}*MeV \
  --outputFile ODD_sim.root
)

