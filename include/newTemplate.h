//Header file that encapsulates the NewTemplate class, which is used to create a new template in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef NEWTEMPLATE_H
#define NEWTEMPLATE_H

#include <QDialog>

namespace Ui {
class NewTemplate;
}

class NewTemplate : public QDialog
{
    Q_OBJECT

public:
    // Constructor for the NewTemplate class
    explicit NewTemplate(QWidget *parent = nullptr);
    ~NewTemplate();

private slots:
    // Slot function that opens the file dialog to save a template file
    void on_saveTemplateButton_clicked();
    //closes the dialog without saving
    void on_cancelTemplateButton_clicked();

private:
    Ui::NewTemplate *ui;
};

#endif // NEWTEMPLATE_H
