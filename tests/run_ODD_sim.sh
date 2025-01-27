#!/bin/bash

# Simulation Parameters
NUMEVENTS=1
ENERGY=65536
PARTICLE="gamma"
ETA_MIN=0.20
ETA_MAX=0.25
PHI_MIN=0
PHI_MAX=6.28318530718 # 2 * pi

# Resolve script directory
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

STEERING_FILE="$SCRIPT_DIR/../steering/OpenDataDetector.py"
GEOMETRY_FILE="$SCRIPT_DIR/../geometry/OpenDataDetector/OpenDataDetector.xml"

# Check if files exist
if [[ ! -f "$STEERING_FILE" ]]; then
  echo "Error: Steering file not found at $STEERING_FILE"
  [[ "$CI" == "true" ]] && exit 1
fi

if [[ ! -f "$GEOMETRY_FILE" ]]; then
  echo "Error: Geometry file not found at $GEOMETRY_FILE"
  [[ "$CI" == "true" ]] && exit 1
fi

# Output directory
mkdir -p "$SCRIPT_DIR/run"

# Set DDFastCaloSim library path
DDFastCaloSim_LIB=${DDFastCaloSim_LIB:-"${SCRIPT_DIR}/../build/libDDFastCaloSim.so"}

# Run the simulation
(
  cd "$SCRIPT_DIR/run" || exit
  LD_PRELOAD=${DDFastCaloSim_LIB} ddsim \
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
exit_status=$?

if [[ $exit_status -ne 0 ]]; then
  echo "Error: Simulation failed with exit status $exit_status"
  [[ "$CI" == "true" ]] && exit $exit_status
else
  echo "Simulation completed successfully."
fi
