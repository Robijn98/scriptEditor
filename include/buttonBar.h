//header file encapulates the button bar class, which is used to manage the buttons in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 10 August 2025 updated to NCCA coding standards


#pragma once

#include <QWidget>
#include <QPushButton>
#include <QToolButton>

#include "editFile.h"
#include "searchAndReplace.h"
#include "openTemplate.h"
#include "scriptEditor.h"
#include "console.h"
#include "tabScriptEditor.h"


namespace Ui {
class ButtonBar;
}

class ButtonBar : public QWidget
{
    Q_OBJECT

public:
/// Constructor for the ButtonBar class
explicit ButtonBar(TabScriptEditor* tabEditor, Console *console, QWidget *parent = nullptr);
    ~ButtonBar();


private slots:

    // Slot functions for button clicks
    void on_saveButton_clicked();

    void on_clearButton_clicked();

    void on_searchAndReplaceButton_clicked();

    void on_templateButton_clicked();

    void on_runButton_clicked();

    void on_runPartialButton_clicked();

    void on_clearTerminalButton_clicked();

private:
    // Private member functions
    Ui::ButtonBar *ui;

    // pointers to classes used in the button bar
    EditFile *editfile;
    SearchAndReplace *searchandreplace;
    OpenTemplate *opentemplate;
    QTabWidget* tabWidget;
    TabScriptEditor* tabEditor;
    Console *console;

    // Function to get the current editor from the tab editor
    CodeEditor* currentEditor() const;

};

