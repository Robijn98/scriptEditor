#include "scriptEditorPanel.h"
// #include "commandList.h"
// #include "config.h"
// #include "style.h"

#include <iostream>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QSplitter>

ScriptEditorPanel::ScriptEditorPanel(QWidget *parent)
    : QDockWidget(parent)
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(container);

    // File button setup
    QHBoxLayout *headerLayout = new QHBoxLayout;
    QToolButton *fileButton = new QToolButton();
    fileButton->setText("File");
    QMenu *fileMenu = new QMenu();
    fileButton->setMenu(fileMenu);
    fileButton->setPopupMode(QToolButton::InstantPopup);

    fileMenu->addAction("New script", this, &ScriptEditorPanel::temp);
    fileMenu->addAction("Open script", this, &ScriptEditorPanel::temp);
    fileMenu->addAction("Save script as", this, &ScriptEditorPanel::temp);
    fileMenu->addAction("Exit", this, &ScriptEditorPanel::temp);

    headerLayout->addWidget(fileButton);
    mainLayout->addLayout(headerLayout);  // ✅ this line is critical

    container->setLayout(mainLayout);
    this->setWidget(container);
}

void ScriptEditorPanel::temp()
{
    std::cout << "Temporary definition\n";
}
