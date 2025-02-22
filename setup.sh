#!/bin/bash

# Use environment variable if set, otherwise use default version
KEY4HEP_VERSION=${KEY4HEP_VERSION:-"2025-01-28"}

# Setup Key4HEP environment only if not already set
if [ -z "$KEY4HEP_STACK" ]; then
    source /cvmfs/sw.hsf.org/key4hep/setup.sh -r "${KEY4HEP_VERSION}"
else
    echo "✅ KEY4HEP_STACK is already set. Skipping Key4HEP environment setup."
fi

# Set up python virtual environment only if it doesn't exist
mkdir -p ~/.local/bin
export PATH=~/.local/bin:"${PATH}"

# Download cvmfs-venv script if not already present
if [ ! -f ~/.local/bin/cvmfs-venv ]; then
    curl -sL https://raw.githubusercontent.com/jbeirer/cvmfs-venv/main/cvmfs-venv.sh -o ~/.local/bin/cvmfs-venv
    chmod +x ~/.local/bin/cvmfs-venv
fi

# Check if the virtual environment already exists
if [ ! -d "fcs-venv" ]; then
    cvmfs-venv fcs-venv
else
    echo "✅ Virtual environment 'fcs-venv' already exists. Skipping creation."
fi

# Check if the virtual environment is already activated
if [ -z "$VIRTUAL_ENV" ]; then
    echo "🔄 Activating virtual environment..."
    . fcs-venv/bin/activate
else
    echo "✅ Virtual environment is already active."
fi

# Check if all required Python dependencies are installed
pip install --quiet --no-dependencies -r requirements.txt

# Check if pre-commit is already installed
if ! pre-commit --version &>/dev/null; then
    echo "🔄 Installing pre-commit hooks..."
    pre-commit install
else
    echo "✅ Pre-commit hooks are already installed."
fi
