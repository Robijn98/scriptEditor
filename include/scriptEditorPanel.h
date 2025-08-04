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
#include "terminal.h"
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
    void newCommand();
    void editCommand();
    void openTemplate();
    void newTemplate();
    void editTemplate();
    void removeTemplate();

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
    Terminal *terminal;
};




