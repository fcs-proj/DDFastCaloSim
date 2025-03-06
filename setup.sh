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

# Capture all environment variables for the current session
# This allows us to use jupyter notebooks in the key4hep environment
ENV_FILE=".env"
echo "🔄 Saving all environment variables to .env file..."

# Use printenv to retrieve all environment variables and format them for a .env file.
#
# Exclusions:
# 1) Exclude PKG_CONFIG_PATH to prevent "execvp(3) failed: Argument list too long."
#    - There is nothing special about PKG_CONFIG_PATH, but it is long and unnecessary.
# 2) Exclude all SINGULARITY and APPTAINER-related variables.
#    - This helps avoid potential conflicts or issues with Singularity/Apptainer.

EXCLUDE_PATTERNS=(
    PKG_CONFIG_PATH 
    ".*SINGULARITY.*"
    ".*APPTAINER.*"
    ALRB_CONT_IMAGE
)
# Convert patterns into a grep-compatible regex
EXCLUDE_REGEX="$(printf "|%s" "${EXCLUDE_PATTERNS[@]}" | cut -c2-)"

printenv | grep -vE "^(${EXCLUDE_REGEX})=" \
    | awk -F= '{print $1"=\"" $2 "\""}' > "$ENV_FILE"

echo "✅ .env file created successfully."
