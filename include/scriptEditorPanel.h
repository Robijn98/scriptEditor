//header file encapsulates the ScriptEditorPanel class, which is used to manage the UI in the application
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

#pragma once

#include "highlighter.h"
#include "scriptEditor.h"
#include "commandList.h"
#include "newCommand.h"
#include "editCommand.h"
#include "buttonBar.h"
#include "editFile.h"
#include "openTemplate.h"
#include "newTemplate.h"
#include "editTemplate.h"
#include "console.h"
#include "tabScriptEditor.h"

#include <QDockWidget>
#include <QTextEdit>
#include <QToolButton>
#include <QMenu>

class ScriptEditorPanel : public QDockWidget
{
    Q_OBJECT

public:
    explicit ScriptEditorPanel(QWidget *parent = nullptr);

private slots:
    //slots that handle opening previously established QWidgts
    void newCommand();
    void editCommand();
    void openTemplate();
    void newTemplate();
    void editTemplate();
    void removeTemplate();


protected:
    // Override the close event to handle closing the script editor panel
    void closeEvent(QCloseEvent* event) override;



private:
    QTabWidget* tabWidget;
    CodeEditor* currentEditor() const;
    TabScriptEditor *tabEditor;
    Highlighter *highlighter;
    CommandList *commandList;
    NewCommand *newcommand;
    EditCommand *editcommand;
    ButtonBar *buttonbar;
    EditFile *editfile;
    OpenTemplate *opentemplate;
    NewTemplate *newtemplate;
    EditTemplate *edittemplate;
    Console *console;
};




