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
    explicit EditTemplate(QWidget *parent = nullptr);
    ~EditTemplate();

private slots:
    void on_browseEditButton_clicked();

    void on_saveEditButton_clicked();

    void on_cancelEditButton_clicked();

private:
    Ui::editTemplate *ui;
};

#endif // EDITTEMPLATE_H
