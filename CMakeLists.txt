# cmake_minimum_required(VERSION 3.22.1)

# # Project Info
# set(PROJECT_NAME helloWorld)
# project(${PROJECT_NAME})

# find_package(Qt5 COMPONENTS Widgets REQUIRED)


# # Project Version
# set(PROJECT_VERSION_MAJOR 1)
# set(PROJECT_VERSION_MINOR 0)
# set(PROJECT_VERSION_PATCH 0)
# set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# # Qt Setup
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# # Environment Validation
# if(NOT DEFINED ENV{DEVKIT_LOCATION})
#     message(FATAL_ERROR "DEVKIT_LOCATION is not set in environment.")
# endif()

# include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# # UI Files
# set(UI_FILES
#     ui/buttonBar.ui
#     ui/commandList.ui
#     ui/editCommand.ui
#     ui/lineNumber.ui
#     # ui/mainWindow.ui
#     ui/newCommand.ui
#     ui/scriptEditor.ui
#     ui/scriptEditorPanel.ui
#     ui/searchAndReplace.ui
#     ui/openTemplate.ui
#     ui/newTemplate.ui
#     ui/editTemplate.ui
# )

# # Source Files
# set(PROJECT_SOURCES
#     src/main.cpp
#     helloWorld.cpp

#     src/mainWindow.cpp
#     include/mainWindow.h

#     src/highlighter.cpp
#     include/highlighter.h

#     src/lineNumber.cpp
#     include/lineNumber.h

#     src/scriptEditor.cpp
#     include/scriptEditor.h

#     src/scriptEditorPanel.cpp
#     include/scriptEditorPanel.h

#     src/buttonBar.cpp
#     include/buttonBar.h

#     src/commandList.cpp
#     include/commandList.h

#     src/editCommand.cpp
#     include/editCommand.h

#     src/newCommand.cpp
#     include/newCommand.h

#     src/searchAndReplace.cpp
#     include/searchAndReplace.h

#     src/openTemplate.cpp
#     include/openTemplate.h

#     src/newTemplate.cpp
#     include/newTemplate.h

#     src/editTemplate.cpp
#     include/editTemplate.h

#     src/editFile.cpp
#     include/editFile.h

#     src/uiExecutable.cpp
#     include/uiExecutable.h

#     include/mayaUtils.h

#     resources.qrc

#     ${UI_FILES}
# )

# # Build Target
# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(ScriptEditor
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# else()
#     if(ANDROID)
#         add_library(ScriptEditor SHARED ${PROJECT_SOURCES})
#     else()
#         add_executable(ScriptEditor ${PROJECT_SOURCES})
#     endif()
# endif()

# target_link_libraries(ScriptEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# # macOS Bundle Properties
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#     set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ScriptEditor)
# endif()

# set_target_properties(ScriptEditor PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )


# target_include_directories(ScriptEditor PRIVATE ${Qt6Widgets_INCLUDE_DIRS})


# # Group Sources in IDE
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES})

# # Installation
# include(GNUInstallDirs)
# install(TARGETS ScriptEditor
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(ScriptEditor)
# endif()

# # Maya Plugin
# set(LIBRARIES
#     OpenMaya Foundation
# )



# build_plugin()

# # Maya Plugin Path
# set(MAYA_PLUGINS_DIR "/home/s5725067/devkitBase/plug-ins/maya2023/plug-ins")

# if(NOT DEFINED MAYA_PLUGINS_DIR)
#     message(FATAL_ERROR "MAYA_PLUGINS_DIR is not set. Please set it manually or in the environment.")
# else()
#     message(STATUS "MAYA_PLUGINS_DIR is set to: ${MAYA_PLUGINS_DIR}")
# endif()


cmake_minimum_required(VERSION 3.22)

project(helloWorld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Correct Qt version: Qt5, not Qt6
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

# ✅ Maya DevKit check
if(NOT DEFINED ENV{DEVKIT_LOCATION})
    message(FATAL_ERROR "DEVKIT_LOCATION is not set in environment.")
endif()

# ✅ Maya plugin CMake helpers
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# ✅ Qt UI processing
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ✅ Source files (simplified)
set(SOURCES
    helloWorld.cpp
    src/uiExecutable.cpp
    include/uiExecutable.h
    # src/scriptEditorPanel.cpp
    # include/scriptEditorPanel.h
)

# ✅ UI files
set(UI_FILES
    ui/scriptEditor.ui
    # Add others as needed
)

# ✅ Plugin target (shared library!)
add_library(ScriptEditor SHARED
    ${SOURCES}
    ${UI_FILES}
)

# ✅ Includes
target_include_directories(ScriptEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
)

# ✅ Link Qt5
target_link_libraries(ScriptEditor PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

# ✅ Maya plugin
set(LIBRARIES OpenMaya Foundation)
build_plugin()

# ✅ Optional: install path (if needed)
set(MAYA_PLUGINS_DIR "/home/s5725067/devkitBase/plug-ins/maya2023/plug-ins")
if(NOT DEFINED MAYA_PLUGINS_DIR)
    message(FATAL_ERROR "MAYA_PLUGINS_DIR is not set. Please set it manually or in the environment.")
else()
    message(STATUS "MAYA_PLUGINS_DIR is set to: ${MAYA_PLUGINS_DIR}")
endif()
