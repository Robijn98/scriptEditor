//header file encapsulates the NewCommand class, which is used to create a new command in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef NEWCOMMAND_H
#define NEWCOMMAND_H

#include <QDialog>

namespace Ui {
class NewCommand;
}

class NewCommand : public QDialog
{
    Q_OBJECT

public:
    // Constructor for the NewCommand class
    explicit NewCommand(QWidget *parent = nullptr);
    ~NewCommand();
    
signals:    
    // Signal emitted when a new command is added
    void commandAdded();

private slots:
    //save the new command to a file
    void on_saveButton_clicked();
    //closes the dialog without saving
    void on_cancelButton_clicked(); 
    //allows you to save your file as a class, showing a input line to put the exposes function in
    void on_classCheckBox_toggled(bool checked);

private:
    Ui::NewCommand *ui;
    
};

#endif // NEWCOMMAND_H
