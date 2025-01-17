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
  exit 1
fi

if [[ ! -f "$GEOMETRY_FILE" ]]; then
  echo "Error: Geometry file not found at $GEOMETRY_FILE"
  exit 1
fi

# Output directory
mkdir -p "$SCRIPT_DIR/run"

# Run the simulation
(
  cd "$SCRIPT_DIR/run" || exit
  if ! ddsim \
    --steeringFile ${STEERING_FILE} \
    --compactFile ${GEOMETRY_FILE} \
    --numberOfEvents ${NUMEVENTS} \
    --gun.particle ${PARTICLE} \
    --gun.etaMin ${ETA_MIN} \
    --gun.etaMax ${ETA_MAX} \
    --gun.phiMin ${PHI_MIN} \
    --gun.phiMax ${PHI_MAX} \
    --gun.energy ${ENERGY}*MeV \
    --outputFile ODD_sim.root; then
    echo "Error: Simulation failed"
    exit 1
  fi
)
exit_status=$?
if [ $exit_status -ne 0 ]; then
  echo "Error: Subshell exited with status $exit_status"
  exit $exit_status
fi