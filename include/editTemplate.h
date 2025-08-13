//header file to encapsulate the EditTemplate class, which is used to edit templates in the script editor tab

#ifndef EDITTEMPLATE_H
#define EDITTEMPLATE_H

#include <QDialog>

namespace Ui {
class editTemplate;
}

class EditTemplate : public QDialog
{
    Q_OBJECT

public:
    // Constructor for the EditTemplate class
    explicit EditTemplate(QWidget *parent = nullptr);
    ~EditTemplate();

private slots:
    //open the file dialog to browse for a template file
    void on_browseEditButton_clicked();
    //save the edited template to a file
    void on_saveEditButton_clicked();
    //close the dialog without saving
    void on_cancelEditButton_clicked();

private:
    Ui::editTemplate *ui;
};

#endif // EDITTEMPLATE_H
