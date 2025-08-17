cmake_minimum_required(VERSION 3.15)
project(besEditor)

# Enable Qt processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)

# Set UI path for .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

# Get Maya SDK path from environment
if(NOT DEFINED ENV{MAYA_LOCATION})
    message(FATAL_ERROR "MAYA_LOCATION environment variable not set")
endif()
set(MAYA_LOCATION $ENV{MAYA_LOCATION})

# Find Qt5 (system Qt is not used by Maya plugin; only for CMake to know headers)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# Include directories
include_directories(
    ${MAYA_LOCATION}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
set(SOURCES
    main.cpp
    src/scriptEditorUtils.cpp
    src/scriptEditorPanel.cpp
    src/newCommand.cpp
    src/buttonBar.cpp
    src/commandList.cpp
    src/editCommand.cpp
    src/editFile.cpp
    src/editTemplate.cpp
    src/highlighter.cpp
    src/lineNumber.cpp
    src/newTemplate.cpp
    src/scriptEditor.cpp
    src/openTemplate.cpp
    src/searchAndReplace.cpp
    src/console.cpp
    src/tabScriptEditor.cpp
    src/config.cpp
    src/mainUserScript.cpp
)

set(HEADERS
    include/scriptEditorUtils.h
    include/scriptEditorPanel.h
    include/newCommand.h
    include/config.h
    include/style.h
    include/buttonBar.h
    include/commandList.h
    include/editFile.h
    include/editTemplate.h
    include/highlighter.h
    include/lineNumber.h
    include/newTemplate.h
    include/scriptEditor.h
    include/openTemplate.h
    include/searchAndReplace.h
    include/editCommand.h
    include/console.h
    include/tabScriptEditor.h
    include/mainUserScript.h
)

set(UI
    ui/newCommand.ui
    ui/scriptEditorPanel.ui
    ui/scriptEditor.ui
    ui/editCommand.ui
    ui/editTemplate.ui
    ui/newTemplate.ui
    ui/openTemplate.ui
    ui/searchAndReplace.ui
    ui/buttonBar.ui
    ui/commandList.ui
    ui/editTemplate.ui
    ui/lineNumber.ui
    ui/mainScript.ui
)

set(RESOURCES
    resources.qrc
)

# Add Maya lib directory
link_directories(${MAYA_LOCATION}/lib)

# Build shared library (plugin)
add_library(besEditor SHARED ${SOURCES} ${HEADERS} ${UI} ${RESOURCES})

# Link Maya and Qt libraries (Maya provides Qt at runtime)
target_link_libraries(besEditor
    Foundation
    OpenMaya
    OpenMayaUI
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

# Copy syntax highlighting files to the build directory
add_custom_target(CopySyntaxFiles ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/syntaxLists
    ${CMAKE_CURRENT_BINARY_DIR}/syntaxLists
    COMMENT "Copying syntax highlighting files to build directory"
)

# Set output plugin .so name
set_target_properties(besEditor PROPERTIES
    PREFIX "lib"
    SUFFIX ".so"
)

# -------------------------------------------------------
# Testing setup
find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

# Build test executable
add_executable(allTests tests/mayaTests.cpp)

# Link against besEditor and Maya libs (avoid system Qt to prevent conflicts)
target_link_libraries(allTests
    besEditor
    GTest::gtest_main
    Foundation
    OpenMaya
    OpenMayaUI
)

# Make sure Maya's lib path is used at runtime
set_target_properties(allTests PROPERTIES
    BUILD_RPATH "${MAYA_LOCATION}/lib"
)

# Discover tests automatically
gtest_discover_tests(allTests)
