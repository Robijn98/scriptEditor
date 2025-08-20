#include <maya/MQtUtil.h>
#include <maya/MGlobal.h>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include "scriptEditorPanel.h"

void showBesEditor()
{
    // Ensure the Maya main window is available
    QMainWindow* mayaMainWindow = qobject_cast<QMainWindow*>(MQtUtil::mainWindow());
    if (!mayaMainWindow) {
        MGlobal::displayError("Could not find Maya main window");
        return;
    }

    QDockWidget* dockWidget = mayaMainWindow->findChild<QDockWidget*>("ScriptEditorPanel");
    ScriptEditorPanel* panel = nullptr;

    if (!dockWidget) {
        dockWidget = new QDockWidget("Bes Editor", mayaMainWindow);
        dockWidget->setObjectName("ScriptEditorPanel");

        panel = new ScriptEditorPanel(dockWidget);
        dockWidget->setWidget(panel);


        mayaMainWindow->addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
    }
    else {
        // Get the existing panel from the existing dock widget
        panel = qobject_cast<ScriptEditorPanel*>(dockWidget->widget());
    }

    dockWidget->show();
    dockWidget->raise();
    dockWidget->setFocus();

    std::cout << "Opening Script Editor" << std::endl;

    // Load the state of the tab editor if it exists
    if (auto* tabEditor = panel->findChild<TabScriptEditor*>()) {
        tabEditor->loadState();
    }
    else
    {
        tabEditor->splitEditor();
    }
    


    // Connect visibility change signal to save state when dock widget is hidden
    QObject::connect(dockWidget, &QDockWidget::visibilityChanged, panel, [panel](bool visible) {
        if (!visible) {
            if (auto* tabEditor = panel->findChild<TabScriptEditor*>()) {
                tabEditor->saveState();
            }
        }
    });
}
