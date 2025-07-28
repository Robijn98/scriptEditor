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
    explicit EditCommand(QWidget *parent = nullptr);
    ~EditCommand();

private slots:
    void on_browseButton_clicked();

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::EditCommand *ui;
};

#endif // EDITCOMMAND_H

