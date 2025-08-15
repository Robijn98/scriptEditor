#!/bin/bash
set -e 

# Default to DEVKIT_LOCATION if not set
if [ -z "$DEVKIT_LOCATION" ]; then
    export DEVKIT_LOCATION="$HOME/devkitBase"
    echo "DEVKIT_LOCATION not set, using default: $DEVKIT_LOCATION"
fi

# Maya plugin output directory
MAYA_PLUGINS_DIR="$DEVKIT_LOCATION/plug-ins/maya2023/plug-ins"
PLUGIN_NAME="besEditor"

echo "Current directory: $(pwd)"
echo "Maya plugins directory: $MAYA_PLUGINS_DIR"

# Ensure plugin output dir exists
if [ ! -d "$MAYA_PLUGINS_DIR" ]; then
    echo "Error: Directory does not exist: $MAYA_PLUGINS_DIR"
    exit 1
fi

# Configure and build
cmake -S . -B build
cmake --build build

# Find the built plugin file (.so)
PLUGIN_FILE_PATH=$(find build -type f -name "lib${PLUGIN_NAME}.so" -o -name "${PLUGIN_NAME}.so" | head -n 1)

if [ -z "$PLUGIN_FILE_PATH" ]; then
    echo "Error: Plugin file not found in build directory"
    exit 1
fi

# Copy plugin to Maya plugins directory
cp "$PLUGIN_FILE_PATH" "$MAYA_PLUGINS_DIR"
echo "Plugin $(basename "$PLUGIN_FILE_PATH") copied to: $MAYA_PLUGINS_DIR"
