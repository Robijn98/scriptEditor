#include <maya/MFnPlugin.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

class MyQtWindow : public QWidget {
public:
    MyQtWindow() {
        setWindowTitle("Qt in Maya");
        auto* btn = new QPushButton("Hello from Qt!", this);
        btn->resize(200, 100);
    }
};

MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, "YourName", "1.0", "Any");

    int argc = 0;
    QCoreApplication* coreApp = QCoreApplication::instance();
    QApplication* app = nullptr;

    if (!coreApp) {
        app = new QApplication(argc, nullptr);
    } else {
        // If coreApp is not null, try to cast to QApplication
        app = qobject_cast<QApplication*>(coreApp);
        if (!app) {
            // This should rarely happen in Maya, but you can handle the error here
            return MS::kFailure;
        }
    }

    // Show the Qt window
    MyQtWindow* win = new MyQtWindow();
    win->show();

    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
    // Clean up if necessary
    return MS::kSuccess;
}
