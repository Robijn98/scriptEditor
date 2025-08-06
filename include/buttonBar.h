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
explicit ButtonBar(TabScriptEditor* tabEditor, Console *console, QWidget *parent = nullptr);
    ~ButtonBar();


private slots:


    void on_saveButton_clicked();

    void on_clearButton_clicked();

    void on_searchAndReplaceButton_clicked();

    void on_templateButton_clicked();

    void on_runButton_clicked();

    void on_runPartialButton_clicked();

    void on_clearTerminalButton_clicked();

private:
    Ui::ButtonBar *ui;
    EditFile *editfile;
    SearchAndReplace *searchandreplace;
    OpenTemplate *opentemplate;
    QTabWidget* tabWidget;
    CodeEditor* currentEditor() const;
    TabScriptEditor* tabEditor;
    Console *console;
};

