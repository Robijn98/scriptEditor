//header file encapsulates the EditCommand class, which is used to edit commands and manage the widget for editing commands
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class EditCommand;
}

class EditCommand : public QDialog
{
    Q_OBJECT

public:
    // Constructor for the EditCommand class
    explicit EditCommand(QWidget *parent = nullptr);
    ~EditCommand();

private slots:
    //slot function that opens the file dialog to browse for a command file
    void on_browseButton_clicked();
    //slot function that closes the dialog without saving
    void on_cancelButton_clicked();
    //slot function that saves the command to a file    
    void on_saveButton_clicked();

private:
    Ui::EditCommand *ui;
};

#endif // EDITCOMMAND_H

