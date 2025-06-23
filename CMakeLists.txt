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
