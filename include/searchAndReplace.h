//header file for the SearchAndReplace class, which is used to manage the search and replace functionality in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

#ifndef SEARCHANDREPLACE_H
#define SEARCHANDREPLACE_H

#include <QDialog>

#include "scriptEditor.h"

namespace Ui {
class SearchAndReplace;
}

class SearchAndReplace : public QDialog
{
    Q_OBJECT

public:
    // Constructor for the SearchAndReplace class
    explicit SearchAndReplace(CodeEditor* editor, QWidget *parent = nullptr);
    ~SearchAndReplace();

private slots:
    //closes the dialog 
    void on_cancelButton_clicked();
    //finds next occurrence of the search term
    void on_findNextButton_clicked();
    //replaces the currently selected occurrence of the search term
    void on_replaceButton_clicked();
    //replaces all occurrences of the search term in the editor
    void on_replaceAllButton_clicked();

private:
    CodeEditor *editor;
    Ui::SearchAndReplace *ui;

};

#endif // SEARCHANDREPLACE_H
