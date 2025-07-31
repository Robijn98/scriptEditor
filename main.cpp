#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "scriptEditorUtils.h"

class ShowScriptEditorCmd : public MPxCommand
{
public:
    static void* creator() { return new ShowScriptEditorCmd(); }
    MStatus doIt(const MArgList&) override {
        showScriptEditorDock(); // Call your panel function
        return MS::kSuccess;
    }
};

// Register the plugin
MStatus initializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, "YourName", "1.0", "Any");
    plugin.registerCommand("showScriptEditorDock", ShowScriptEditorCmd::creator);
    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);
    plugin.deregisterCommand("showScriptEditorDock");
    return MS::kSuccess;
}
