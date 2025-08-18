//header file that encapsulates the MainUserScript class, which is used to save the main script
//author: Robin van den Eerenbeemd
// version: 1.0
// date: 13 August 2025 updated to NCCA coding standards

#ifndef MAINUSERSCRIPT_H
#define MAINUSERSCRIPT_H

#include <QDialog>

namespace Ui {
    class SaveMainScript;
}
class MainUserScript : public QDialog
{
    Q_OBJECT
public:
    explicit MainUserScript(QWidget *parent = nullptr);
    ~MainUserScript();
private slots:
    // Slot function that saves the main script to a file
    void on_saveMainScriptButton_clicked();
    // Slot function that closes the dialog without saving
    void on_cancelMainScriptButton_clicked();
private:
    Ui::SaveMainScript *ui;
};

#endif // MAINUSERSCRIPT_H