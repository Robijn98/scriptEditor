#include "scriptEditorPanel.h"
#include "commandList.h"
#include "config.h"
#include "style.h"

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


    // ----------------ADD ALL PARTS ----------------
    editor = new CodeEditor(container);
    commandList = new CommandList();
    editfile = new EditFile(editor);
    buttonbar = new ButtonBar(editor);
    newcommand = new NewCommand(this);
    editcommand = new EditCommand(this);
    highlighter = new Highlighter(editor->document());
    // opentemplate = new OpenTemplate(editor);
    newtemplate = new NewTemplate();
    edittemplate = new EditTemplate();

    QSplitter *scriptEditorSplitter = new QSplitter;

    // MAIN LAYOUT
    QHBoxLayout *headerLayout = new QHBoxLayout;

    //-----------------------FILE MENU -----------------------------
    QToolButton *fileButton = new QToolButton();
    fileButton->setText("File");
    QMenu *fileMenu = new QMenu();
    fileButton->setMenu(fileMenu);
    fileButton->setPopupMode(QToolButton::InstantPopup);

    fileMenu->addAction("New script", editfile,  &EditFile::newFile);

    fileMenu->addAction("Open script", editfile, &EditFile::openFile);

    fileMenu->addAction("Save script as", editfile, &EditFile::saveFile);

    fileMenu->addAction("Exit", editfile, &EditFile::exitApp);


    //----------------------COMMAND MENU -----------------------------

    QToolButton *commandButton = new QToolButton();
    commandButton->setText("Commands");
    QMenu *commandMenu = new QMenu();
    commandButton->setMenu(commandMenu);
    commandButton->setPopupMode(QToolButton::InstantPopup);

    QAction* addAction = commandMenu->addAction("Add new");
    connect(addAction, &QAction::triggered, this, &ScriptEditorPanel::newCommand);

    commandMenu->addAction("Edit excisting", this, &ScriptEditorPanel::editCommand);

    QAction* renameAction = commandMenu->addAction("Rename");
    connect(renameAction, &QAction::triggered, commandList, &CommandList::rename);
    connect(renameAction, &QAction::triggered, commandList, &CommandList::refreshList);

    QAction* removeAction = commandMenu->addAction("Remove");
    connect(removeAction, &QAction::triggered, commandList, &CommandList::remove);
    connect(removeAction, &QAction::triggered, commandList, &CommandList::refreshList);

    QAction* refreshAction = commandMenu->addAction("Refresh");
    connect(refreshAction, &QAction::triggered, commandList, &CommandList::refreshList);


    //----------------------TEMPLATE MENU -----------------------------
    QToolButton *templateButton = new QToolButton();
    templateButton->setText("Templates");
    QMenu *templateMenu = new QMenu();
    templateButton->setMenu(templateMenu);
    templateButton->setPopupMode(QToolButton::InstantPopup);

    templateMenu->addAction("Load Template", this, &ScriptEditorPanel::openTemplate);

    templateMenu->addAction("Add Template", this, &ScriptEditorPanel::newTemplate);

    templateMenu->addAction("Edit Template", this, &ScriptEditorPanel::editTemplate);

    templateMenu->addAction("Remove Template", this, &ScriptEditorPanel::removeTemplate);


    //---------------------- LAYOUT -----------------------------

    fileButton->setMinimumWidth(80);
    commandButton->setMinimumWidth(80);
    templateButton->setMinimumWidth(80);

    headerLayout->addWidget(fileButton);
    headerLayout->addWidget(commandButton);
    headerLayout->addWidget(templateButton);

    headerLayout->addStretch();

    //CommandBox
    scriptEditorSplitter->addWidget(commandList);
    scriptEditorSplitter->addWidget(editor);
    scriptEditorSplitter->setSizes({75,250});

    QVBoxLayout *mainLayout = new QVBoxLayout(container);

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(buttonbar);
    mainLayout->addWidget(scriptEditorSplitter);

    container->setLayout(mainLayout);
    this->setWidget(container);


    //---------------------- COMMAND LIST -----------------------------
    QDir dir(Config::riggingCommandsPath);

    connect(commandList, &CommandList::commandSelected, editor, [=](const QString &text) {
        //check if you still need to append path
        QString currentText = editor->toPlainText();
        QString sysImport = QString("sys.path.append('%1')").arg(dir.absolutePath());

        if(!currentText.contains("import sys"))
            {
                editor->appendPlainText("import sys");
            }
        if(!currentText.contains(sysImport))
            {
                editor->appendPlainText(sysImport);
                editor->appendPlainText("\n");
            }

        editor->appendPlainText(text);
    });


    //------------------------------- STYLE -------------------------------
    container->setStyleSheet(Style::containerStyle);

    //buttons
    commandButton->setStyleSheet(Style::buttonStyle);

    fileButton->setStyleSheet(Style::buttonStyle);

    templateButton->setStyleSheet(Style::buttonStyle);

    //menu
    commandMenu->setStyleSheet(Style::menuStyle);

    fileMenu->setStyleSheet(Style::menuStyle);

    templateMenu->setStyleSheet(Style::menuStyle);

    commandList->setStyleSheet(Style::listStyle);

}


void ScriptEditorPanel::temp()
{
    std::cout<<"temporary defintion/n";
}


void ScriptEditorPanel::newCommand()
{
    newcommand->show();
    newcommand->raise();
    newcommand->setFocus();
}


void ScriptEditorPanel::editCommand()
{
    editcommand->show();
    editcommand->raise();
    editcommand->setFocus();
}

void ScriptEditorPanel::openTemplate()
{
    OpenTemplate* opentemplate = new OpenTemplate(editor, TemplateMode::Load);
    opentemplate->loadList();
    opentemplate->show();
    opentemplate->raise();
    opentemplate->setFocus();
}

void ScriptEditorPanel::newTemplate()
{
    newtemplate->show();
    newtemplate->raise();
    newtemplate->setFocus();
}

void ScriptEditorPanel::editTemplate()
{

    edittemplate->show();
    edittemplate->raise();
    edittemplate->setFocus();
}

void ScriptEditorPanel::removeTemplate()
{
    OpenTemplate* opentemplate = new OpenTemplate(editor, TemplateMode::Remove);
    opentemplate->loadList();
    opentemplate->show();
    opentemplate->raise();
    opentemplate->setFocus();
}
