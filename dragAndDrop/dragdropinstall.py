import os
import shutil
import maya.cmds as cmds
import sys

# Determine plugin filename based on OS
if sys.platform == "win32":
    PLUGIN_NAME = "besEditor.mll"
    print("Detected Windows platform")
else:
    PLUGIN_NAME = "libbesEditor.so"
    print("Detected Linux platform")

SCRIPTS_DIR_NAME = "scripts"
USER_SETUP_PATH = os.path.join(cmds.internalVar(userAppDir=True), "userSetup.py") 

def install_plugin():
    # Get Maya directories
    user_app_dir = cmds.internalVar(userAppDir=True)
    scripts_dir = os.path.join(user_app_dir, "scripts")
    plug_ins_dir = os.path.join(user_app_dir, "plug-ins")

    # Ensure plug-ins directory exists
    os.makedirs(plug_ins_dir, exist_ok=True)
    print(f"Ensured plug-ins directory exists: {plug_ins_dir}")

    # Copy plugin to plug-ins folder
    plugin_source = os.path.join(os.path.dirname(__file__), PLUGIN_NAME)
    plugin_dest = os.path.join(plug_ins_dir, PLUGIN_NAME)
    if not os.path.exists(plugin_source):
        raise FileNotFoundError(f"Plugin file not found: {plugin_source}")
    if not os.path.exists(plugin_dest):
        shutil.copy(plugin_source, plugin_dest)
        print(f"Copied {PLUGIN_NAME} to {plugin_dest}")
    else:
        print(f"Plugin already exists in plug-ins folder: {plugin_dest}")

    # Load plugin automatically
    try:
        if not cmds.pluginInfo(PLUGIN_NAME, query=True, loaded=True):
            cmds.loadPlugin(plugin_dest)
            print(f"Loaded plugin: {PLUGIN_NAME}")
    except Exception as e:
        print(f"Error loading plugin: {e}")

    # Create additional script directories
    for subdir in ["riggingCommands", "riggingTemplates", "syntaxLists"]:
        path = os.path.join(scripts_dir, subdir)
        os.makedirs(path, exist_ok=True)
        print(f"Ensured directory exists: {path}")

    # Copy syntax files
    syntax_files = ["maya_cmds_list.txt", "rigging_commands_list.txt"]
    syntax_dir = os.path.join(scripts_dir, "syntaxLists")
    for file_name in syntax_files:
        source_file = os.path.join(os.path.dirname(__file__), file_name)
        dest_file = os.path.join(syntax_dir, file_name)
        if os.path.exists(source_file) and not os.path.exists(dest_file):
            shutil.copy(source_file, dest_file)
            print(f"Copied {file_name} to {syntax_dir}")

    # Copy icon
    icon_source = os.path.join(os.path.dirname(__file__), "bes.png")
    icon_dest = os.path.join(user_app_dir, "prefs", "icons", "bes.png")
    if os.path.exists(icon_source):
        os.makedirs(os.path.dirname(icon_dest), exist_ok=True)
        shutil.copy(icon_source, icon_dest)
        print(f"Copied icon to {icon_dest}")
    else:
        print("Warning: Icon not found")

    # Create shelf
    if not cmds.shelfLayout("BesEditor", exists=True):
        cmds.shelfLayout("BesEditor", parent="ShelfLayout")
        print("Created BesEditor shelf")

    if not cmds.shelfButton("BesEditorButton", exists=True):
        cmds.shelfButton(
            "BesEditorButton",
            label="BesEditor",
            command='cmds.showBesEditor()',
            image="bes.png",  
            parent="BesEditor"
        )
        print("Created BesEditor shelf button")

def onMayaDroppedPythonFile(*args):
    try:
        install_plugin()
        print("Plugin installed successfully.")
    except Exception as e:
        print(f"Error installing plugin: {e}")