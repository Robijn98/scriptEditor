cmake_minimum_required(VERSION 3.15)
project(MayaQtPlugin)

# Enable Qt processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set UI path for .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

# Get Maya SDK path from environment
if(NOT DEFINED ENV{MAYA_LOCATION})
    message(FATAL_ERROR "MAYA_LOCATION environment variable not set")
endif()
set(MAYA_LOCATION $ENV{MAYA_LOCATION})

# Find Qt5
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# Include directories
include_directories(
    ${MAYA_LOCATION}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files (no .h or .ui in SOURCES!)
set(SOURCES
    main.cpp
    src/myDialog.cpp
    src/scriptEditorUtils.cpp
    src/scriptEditorPanel.cpp
)

set(HEADERS
    include/myDialog.h
    include/scriptEditorUtils.h
    include/scriptEditorPanel.h
)


# Add Maya lib directory
link_directories(${MAYA_LOCATION}/lib)

# Build shared library
add_library(MayaQtPlugin SHARED ${SOURCES} ${HEADERS})

# Link Maya and Qt libraries
target_link_libraries(MayaQtPlugin
    Foundation
    OpenMaya
    OpenMayaUI
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui

)

# Set output plugin .so name
set_target_properties(MayaQtPlugin PROPERTIES
    PREFIX "lib"
    SUFFIX ".so"
)
