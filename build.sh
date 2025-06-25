#!/bin/bash
set -e

# Set plugin output directory
MAYA_PLUGINS_DIR="$HOME/devkitBase/plug-ins/maya2023/plug-ins"

# Print directories
CURRENT_DIR=$(pwd)
echo "Current directory: $CURRENT_DIR"
echo "Maya plugins directory: $MAYA_PLUGINS_DIR"

# Check if target directory exists
if [ ! -d "$MAYA_PLUGINS_DIR" ]; then
    echo "Error: Directory does not exist: $MAYA_PLUGINS_DIR"
    exit 1
fi

# Configure and build the plugin
cmake -S . -B build
cmake --build build

# Define expected output file (adjust if needed)
PLUGIN_NAME="helloWorld"
PLUGIN_FILE="${PLUGIN_NAME}.so"

# Check if plugin file exists
if [ ! -f "build/${PLUGIN_FILE}" ]; then
    echo "Error: Plugin file not found: build/${PLUGIN_FILE}"
    exit 1
fi

# Copy plugin to Maya plugins directory
cp "build/${PLUGIN_FILE}" "$MAYA_PLUGINS_DIR"
echo "Plugin ${PLUGIN_FILE} copied to $MAYA_PLUGINS_DIR"
