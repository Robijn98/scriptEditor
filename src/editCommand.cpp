#include "editCommand.h"
#include "ui_editCommand.h"

#include <QTextStream>
#include <QMessageBox>

#include "config.h"
#include "style.h"

EditCommand::EditCommand(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditCommand)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit rigging command");

    this->setStyleSheet(Style::containerStyle);

    ui->cancelButton->setStyleSheet(Style::buttonStyle);

    ui->saveButton->setStyleSheet(Style::buttonStyle);

    ui->saveButton->setStyleSheet(Style::buttonStyle);

    ui->browseButton->setStyleSheet(Style::buttonStyle);
}

EditCommand::~EditCommand()
{
    delete ui;
}

void EditCommand::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", Config::riggingCommandsPath, "All Files (*.*)");
    if (!fileName.isEmpty()) {
        ui->label->setText(fileName);
    }
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        ui->editor->setPlainText(in.readAll());
        file.close();
    }
}


void EditCommand::on_saveButton_clicked()
{
    QString fileName = ui->label->text();
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ui->editor->toPlainText();
        file.close();
        this->close();

    }
}


void EditCommand::on_cancelButton_clicked()
{
    this->close();
}
