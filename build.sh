#!/bin/bash

# Fail on error
set -e

# Default to DEVKIT_LOCATION if not set
if [ -z "$DEVKIT_LOCATION" ]; then
    export DEVKIT_LOCATION="$HOME/devkitBase"
    echo "DEVKIT_LOCATION not set, using default: $DEVKIT_LOCATION"
fi

# Maya plugin output directory
MAYA_PLUGINS_DIR="$DEVKIT_LOCATION/plug-ins/maya2023/plug-ins"
PLUGIN_NAME="libMayaQtPlugin"
PLUGIN_FILE="${PLUGIN_NAME}.so"

# Print directory info
echo "Current directory: $(pwd)"
echo "Maya plugins directory: $MAYA_PLUGINS_DIR"

# Ensure plugin output dir exists
if [ ! -d "$MAYA_PLUGINS_DIR" ]; then
    echo "Error: Directory does not exist: $MAYA_PLUGINS_DIR"
    exit 1
fi

# Configure and build
cmake -S . -B build || { echo "CMake configuration failed"; exit 1; }
cmake --build build || { echo "Build failed"; exit 1; }

# Validate plugin file
if [ ! -f "build/${PLUGIN_FILE}" ]; then
    echo "Error: Plugin file not found: build/${PLUGIN_FILE}"
    exit 1
fi

# Copy plugin to Maya plugins directory
cp "build/${PLUGIN_FILE}" "$MAYA_PLUGINS_DIR"
echo "✅ Plugin ${PLUGIN_FILE} copied to: $MAYA_PLUGINS_DIR"