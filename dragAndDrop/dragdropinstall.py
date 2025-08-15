import os
import shutil
import maya.cmds as cmds

PLUGIN_NAME = "libbesEditor.so"
SCRIPTS_DIR_NAME = "scripts"

def install_plugin():
    # Maya-wide user directory (not version-specific)
    maya_dir = os.path.join(os.path.expanduser("~"), "Documents", "maya")
    scripts_dir = os.path.join(maya_dir, SCRIPTS_DIR_NAME)
    plugin_dir = os.path.join(maya_dir, "plug-ins")
    os.makedirs(plugin_dir, exist_ok=True)
    os.makedirs(scripts_dir, exist_ok=True)
    print(f"Maya-wide plugin directory: {plugin_dir}")
    print(f"Maya-wide scripts directory: {scripts_dir}")

    # Copy plugin
    plugin_source = os.path.join(os.path.dirname(__file__), PLUGIN_NAME)
    if not os.path.exists(plugin_source):
        raise FileNotFoundError(f"Could not find plugin file: {plugin_source}")

    plugin_dest = os.path.join(plugin_dir, PLUGIN_NAME)
    shutil.copy(plugin_source, plugin_dest)
    print(f"Copied {PLUGIN_NAME} to {plugin_dest}")

    # Load plugin automatically
    if not cmds.pluginInfo(PLUGIN_NAME, query=True, loaded=True):
        cmds.loadPlugin(PLUGIN_NAME)
        print(f"Loaded plugin: {PLUGIN_NAME}")

    # Create directories for rigging & syntax
    for subdir in ["riggingCommands", "riggingTemplates", "syntaxLists"]:
        path = os.path.join(scripts_dir, subdir)
        os.makedirs(path, exist_ok=True)
        print(f"Ensured directory exists: {path}")

    # Copy syntax highlighting files
    syntax_files = ["maya_cmds_list.txt", "rigging_commands_list.txt"]
    syntax_dir = os.path.join(scripts_dir, "syntaxLists")
    for file_name in syntax_files:
        source_file = os.path.join(os.path.dirname(__file__), file_name)
        dest_file = os.path.join(syntax_dir, file_name)
        if os.path.exists(source_file) and not os.path.exists(dest_file):
            shutil.copy(source_file, dest_file)
            print(f"Copied {file_name} to {syntax_dir}")

    # Copy icon into Maya-wide prefs/icons directory
    icon_source = os.path.join(os.path.dirname(__file__), "bes.png")
    icon_dest = os.path.join(maya_dir, "prefs", "icons", "bes.png")
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
            image="bes.png",  # Maya looks for it in prefs/icons
            parent="BesEditor"
        )
        print("Created BesEditor shelf button")

def onMayaDroppedPythonFile(*args):
    try:
        install_plugin()
        print("Plugin installed successfully.")
    except Exception as e:
        print(f"Error installing plugin: {e}")
