cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME helloWorld)
project(${PROJECT_NAME})

include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

set(SOURCE_FILES
    helloWorld.cpp
)

set(LIBRARIES
    OpenMaya Foundation
)

build_plugin()



#maya
set(MAYA_PLUGINS_DIR "/home/s5725067/devkitBase/plug-ins/maya2023/plug-ins")

if(NOT DEFINED MAYA_PLUGINS_DIR)
    message(FATAL_ERROR "MAYA_PLUGINS_DIR is not set. Please set it manually or in the environment.")
else()
    message(STATUS "MAYA_PLUGINS_DIR is set to: ${MAYA_PLUGINS_DIR}")
endif()



