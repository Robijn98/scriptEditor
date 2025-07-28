#pragma once

#include <QWidget>
#include <QPushButton>
#include <QToolButton>

#include "editFile.h"
#include "searchAndReplace.h"
#include "editTemplate.h"

namespace Ui {
class ButtonBar;
}

class ButtonBar : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonBar(CodeEditor* editor, QWidget *parent = nullptr);
    ~ButtonBar();


private slots:


    void on_saveButton_clicked();

    void on_clearButton_clicked();

    void on_searchAndReplaceButton_clicked();

    void on_templateButton_clicked();

private:
    Ui::ButtonBar *ui;
    EditFile *editfile;
    SearchAndReplace *searchandreplace;
    EditTemplate *edittemplate;

};

