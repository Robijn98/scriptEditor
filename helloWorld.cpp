// #include <stdio.h>
// #include <iostream>
// #include <maya/MString.h>
// #include <maya/MArgList.h>
// #include <maya/MFnPlugin.h>
// #include <maya/MPxCommand.h>
// #include <maya/MIOStream.h>

// // #include <maya/MQtUtil.h>
// // #include <QtWidgets/QMainWindow>
// // #include <QtWidgets/QDockWidget>
// // #include <QtWidgets/QWidget>

// #include "include/uiExecutable.h"
// // #include "include/mayaUtils.h"
// // #include "include/scriptEditorPanel.h"

#include <stdio.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MIOStream.h>

using namespace std;


// void openPanel()
// {
//     static ScriptEditorPanel* panel = nullptr;

//     QWidget* mayaMainWindow = getMayaMainWindow();
//     QMainWindow* mainWindow = qobject_cast<QMainWindow*>(mayaMainWindow);

//     if (!panel && mainWindow) {
//         panel = new ScriptEditorPanel(mainWindow);
//         mainWindow->addDockWidget(Qt::RightDockWidgetArea, panel);
//     }

//     if(panel) {
//         panel->show();
//         panel->raise();
//         panel->activateWindow();
//     }
// }

class helloWorld : public MPxCommand
{
public:
    MStatus doIt( const MArgList& args ) override;
    static void* creator();
};

MStatus helloWorld::doIt( const MArgList& args ) {
    cout << "im a plugIn, hi " << args.asString( 0 ).asChar() << endl;
    // openPanel();
    return MS::kSuccess;
}

void* helloWorld::creator() {
    return new helloWorld;
}

MStatus initializePlugin( MObject obj ) {
    MFnPlugin plugin( obj, "Autodesk", "1.0", "Any" );
    plugin.registerCommand( "HelloWorld", helloWorld::creator );
    return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj ) {
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "HelloWorld");
    return MS::kSuccess;
}
