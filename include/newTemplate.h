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
    explicit NewTemplate(QWidget *parent = nullptr);
    ~NewTemplate();

private slots:
    void on_saveTemplateButton_clicked();

    void on_cancelTemplateButton_clicked();

private:
    Ui::NewTemplate *ui;
};

#endif // NEWTEMPLATE_H
