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
#include <QToolButton>
#include <QMenu>
#include <QDir>


ScriptEditorPanel::ScriptEditorPanel(QWidget *parent)
    : QDockWidget(parent)
{
    QWidget *container = new QWidget(parent);
    
    tabEditor = new TabScriptEditor(container);

    // ----------------ADD ALL PARTS ----------------
    commandList = new CommandList();
    editfile = new EditFile(tabEditor);
    console = new Console(container);

    buttonbar = new ButtonBar(tabEditor, console);
    newcommand = new NewCommand(this);
    editcommand = new EditCommand(this);

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
    //connect signal after newCommand is done
    connect(newcommand, &NewCommand::commandAdded, commandList, &CommandList::refreshList);
    
    commandMenu->addAction("Edit excisting", this, &ScriptEditorPanel::editCommand);

    QAction* renameAction = commandMenu->addAction("Rename");
    connect(renameAction, &QAction::triggered, commandList, &CommandList::rename);
    connect(renameAction, &QAction::triggered, commandList, &CommandList::refreshList);

    QAction* removeAction = commandMenu->addAction("Remove");
    connect(removeAction, &QAction::triggered, commandList, &CommandList::remove);
    connect(removeAction, &QAction::triggered, commandList, &CommandList::refreshList);

    QAction* mainScriptAction = commandMenu->addAction("Save Main Script");
    connect(mainScriptAction, &QAction::triggered, this, &ScriptEditorPanel::saveMainScript);


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



    //--------------------------TAB MENU ---------------------------------------------------
    QToolButton *tabButton = new QToolButton();
    tabButton->setText("tabs");
    QMenu *tabMenu = new QMenu();
    tabButton->setMenu(tabMenu);
    tabButton->setPopupMode(QToolButton::InstantPopup);

    tabMenu->addAction("split screen",tabEditor ,&TabScriptEditor::splitEditor);
    tabMenu->addAction("close all tabs", tabEditor, &TabScriptEditor::closeAllTabs);
    //after all tabs are closed, split screen, but only if close all tabs button is clicked
    
    //only connect if the close all tabs button was pressed
    connect(tabEditor, &TabScriptEditor::allTabsClosed, tabEditor, &TabScriptEditor::splitEditor);
    
    tabMenu->addAction("rename tab", tabEditor, &TabScriptEditor::renameTab);


    
    //------------------------- INITIALIZE LAYoUT MENU --------------------------------------
    QToolButton *layoutButton = new QToolButton();
    layoutButton->setText("Layout");
    QMenu *layoutMenu = new QMenu();
    layoutButton->setMenu(layoutMenu);
    layoutButton->setPopupMode(QToolButton::InstantPopup);



    //---------------------- LAYOUT -----------------------------

    fileButton->setMinimumWidth(80);
    commandButton->setMinimumWidth(80);
    templateButton->setMinimumWidth(80);

    headerLayout->addWidget(fileButton);
    headerLayout->addWidget(commandButton);
    headerLayout->addWidget(templateButton);
    headerLayout->addWidget(tabButton);
    headerLayout->addWidget(layoutButton);


    headerLayout->addStretch();


    // Editor Splitter Layout
    scriptEditorSplitter->addWidget(commandList);
    scriptEditorSplitter->addWidget(tabEditor);
    scriptEditorSplitter->setSizes({75,250});

    
    //console split
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    QVBoxLayout *baseLayout = new QVBoxLayout();

    baseLayout->addLayout(headerLayout);
    baseLayout->addWidget(buttonbar);
    baseLayout->addWidget(scriptEditorSplitter);

    QSplitter *consoleSplitter = new QSplitter(Qt::Vertical);

    //wrap base into a widget
    QWidget *baseWidget = new QWidget();
    baseWidget->setLayout(baseLayout);

    consoleSplitter->addWidget(baseWidget);
    consoleSplitter->addWidget(console);

    consoleSplitter->setSizes({250, 50});

    mainLayout->addWidget(consoleSplitter);

    container->setLayout(mainLayout);
    this->setWidget(container);

    //-----------------------------LAYOUT MENU ----------------------------------------------------


    QAction *buttonBarShowAction = layoutMenu->addAction("Show Button Bar");
    buttonBarShowAction->setCheckable(true);
    buttonBarShowAction->setChecked(true);

    connect(buttonBarShowAction, &QAction::toggled, buttonbar, &ButtonBar::setVisible);

    QAction *consoleShowAction = layoutMenu->addAction("Show Console");
    consoleShowAction->setCheckable(true);
    consoleShowAction->setChecked(true);

    connect(consoleShowAction, &QAction::toggled, console, &Console::setVisible);

    
    QAction *commandListShowAction = layoutMenu->addAction("Show Command List");
    commandListShowAction->setCheckable(true);
    commandListShowAction->setChecked(true);
    connect(commandListShowAction, &QAction::toggled, commandList, &CommandList::setVisible);
    
    
    
    QAction *swapAction = layoutMenu->addAction("Swap Console and Editor");
    swapAction->setCheckable(true);
    swapAction->setChecked(false);
    
    connect(swapAction, &QAction::toggled, this, [=](bool checked) {
        if (checked) {
            // Swap: put baseWidget first, console second
            consoleSplitter->insertWidget(1, baseWidget);
            consoleSplitter->insertWidget(0, console);
        } else {
            // Restore original order: console first, baseWidget second
            consoleSplitter->insertWidget(1, console);
            consoleSplitter->insertWidget(0, baseWidget);
        }
    });



    //---------------------- COMMAND SELECTION -----------------------------
    QDir dir(Config::riggingCommandsPath);
    connect(commandList, &CommandList::commandSelected, this, [=](const QString &text) {
        CodeEditor* editor = currentEditor();
        if (!editor)
        {
            std::cout<<"no";
            return;
        }

        QString currentText = editor->toPlainText();
        QString sysImport = QString("sys.path.append('%1')").arg(dir.absolutePath());

        if (!currentText.contains("import sys")) {
            editor->appendPlainText("import sys");
        }

        if (!currentText.contains(sysImport)) {
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

    tabButton->setStyleSheet(Style::buttonStyle);

    layoutButton->setStyleSheet(Style::buttonStyle);

    //menu
    commandMenu->setStyleSheet(Style::menuStyle);

    fileMenu->setStyleSheet(Style::menuStyle);

    templateMenu->setStyleSheet(Style::menuStyle);

    commandList->setStyleSheet(Style::listStyle);

    tabMenu->setStyleSheet(Style::menuStyle);

    layoutMenu->setStyleSheet(Style::menuStyle);
    

}




CodeEditor* ScriptEditorPanel::currentEditor() const
{
    if (!tabEditor) return nullptr;
    return tabEditor->currentEditor();
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
    CodeEditor* editor = currentEditor();
    if (!editor) return;

    OpenTemplate* opentemplate = new OpenTemplate(tabEditor, TemplateMode::Load);
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
    CodeEditor* editor = currentEditor();
    if (!editor) return;

    OpenTemplate* opentemplate = new OpenTemplate(tabEditor, TemplateMode::Remove);
    opentemplate->loadList();
    opentemplate->show();
    opentemplate->raise();
    opentemplate->setFocus();
}

void ScriptEditorPanel::closeEvent(QCloseEvent* event)
{
    if (auto tabEditor = findChild<TabScriptEditor*>()) {
        tabEditor->saveState();
    }
    QDockWidget::closeEvent(event);
}

void ScriptEditorPanel::saveMainScript()
{
    MainUserScript *mainScriptDialog = new MainUserScript(this);
    mainScriptDialog->show();
    mainScriptDialog->raise();
    mainScriptDialog->setFocus();
}