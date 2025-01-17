#!/bin/bash

# Use environment variable if set, otherwise use default version
KEY4HEP_VERSION=${KEY4HEP_VERSION:-"2024-11-28"}

# Setup key4hep environment
source /cvmfs/sw.hsf.org/key4hep/setup.sh -r "${KEY4HEP_VERSION}"