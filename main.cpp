#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "scriptEditorUtils.h"
#include <maya/MMessage.h>
#include "config.h"
#include <QDebug>

class ShowScriptEditorCmd : public MPxCommand
{
public:
    static void* creator() { return new ShowScriptEditorCmd(); }
    MStatus doIt(const MArgList&) override {
        showBesEditor(); // Call your panel function
        return MS::kSuccess;
    }
};



MStatus initializePlugin(MObject obj)
{
    MStatus status;

    // Initialize configuration paths
    Config::initialize();

    MFnPlugin plugin(obj, "RobinVanDenEerenbeemd", "1.0", "Any");
    status = plugin.registerCommand("showBesEditor", ShowScriptEditorCmd::creator);
    if (!status) {
        status.perror("registerCommand failed");
        return status;
    }

    // Optional: print paths to verify
    qDebug() << "Syntax file:" << Config::syntaxPath;
    qDebug() << "Rigging commands dir:" << Config::riggingCommandsPath;
    qDebug() << "Plugin file:" << Config::pluginFilePath;

    return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);
    plugin.deregisterCommand("showBesEditor");
    //print to maya
    MGlobal::displayInfo("Bes Editor plugin unregistered successfully.");
    return MS::kSuccess;
}
