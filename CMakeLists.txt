cmake_minimum_required(VERSION 3.15)
project(MayaQtPlugin)

# Get Maya SDK path from environment
set(MAYA_LOCATION $ENV{MAYA_LOCATION})

# Find Qt5 Widgets
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Include Maya headers and Qt headers
include_directories(
    ${MAYA_LOCATION}/include
    ${Qt5Widgets_INCLUDE_DIRS}
)

# Add Maya lib directory to linker search path
link_directories(${MAYA_LOCATION}/lib)

# Build shared library
add_library(MayaQtPlugin SHARED main.cpp)

# Link against Maya shared libs and Qt Widgets
target_link_libraries(MayaQtPlugin
    Foundation
    OpenMaya
    Qt5::Widgets
)

# Set output name and suffix for Linux shared object plugin
set_target_properties(MayaQtPlugin PROPERTIES
    PREFIX "lib"
    SUFFIX ".so"
)
