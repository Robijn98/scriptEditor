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
    explicit SearchAndReplace(CodeEditor* editor, QWidget *parent = nullptr);
    ~SearchAndReplace();

private slots:
    void on_cancelButton_clicked();

    void on_findNextButton_clicked();

    void on_replaceButton_clicked();

    void on_replaceAllButton_clicked();

private:
    CodeEditor *editor;
    Ui::SearchAndReplace *ui;

};

#endif // SEARCHANDREPLACE_H
