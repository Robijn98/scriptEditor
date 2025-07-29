#include <maya/MQtUtil.h>
#include <maya/MGlobal.h>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include "scriptEditorPanel.h"

void showScriptEditorDock()
{
    QMainWindow* mayaMainWindow = qobject_cast<QMainWindow*>(MQtUtil::mainWindow());
    if (!mayaMainWindow) {
        MGlobal::displayError("Could not find Maya main window");
        return;
    }

    // Check and remove existing
    QObject* existing = mayaMainWindow->findChild<QObject*>("ScriptEditorPanel");
    if (existing) {
        existing->deleteLater();
    }

    QDockWidget* dockWidget = new QDockWidget("PinkMatcha Script Editor", mayaMainWindow);
    dockWidget->setObjectName("ScriptEditorPanel");

    ScriptEditorPanel* panel = new ScriptEditorPanel(dockWidget);
    dockWidget->setWidget(panel);

    mayaMainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget); // Or any side you want
    dockWidget->show();
    dockWidget->raise();
    dockWidget->setFocus();

    std::cout << "Script Editor Dock shown dockieboy" << std::endl;
}
