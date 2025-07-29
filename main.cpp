// #include <maya/MFnPlugin.h>
// #include <QtWidgets/QApplication>
// #include "myDialog.h"

// MyDialog* g_dialog = nullptr;

// MStatus initializePlugin(MObject obj)
// {
//     MFnPlugin plugin(obj, "YourName", "1.0", "Any");

//     int argc = 0;
//     QCoreApplication* coreApp = QCoreApplication::instance();
//     QApplication* app = nullptr;

//     if (!coreApp) {
//         app = new QApplication(argc, nullptr);
//     } else {
//         app = qobject_cast<QApplication*>(coreApp);
//         if (!app) return MS::kFailure;
//     }

//     if (!g_dialog) {
//         g_dialog = new MyDialog();
//         g_dialog->setAttribute(Qt::WA_DeleteOnClose);
//         g_dialog->show();
//     }

//     return MS::kSuccess;
// }

// MStatus uninitializePlugin(MObject obj)
// {
//     if (g_dialog) {
//         g_dialog->close();
//         g_dialog = nullptr;
//     }

//     return MS::kSuccess;
// }
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
