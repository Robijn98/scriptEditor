#include "editTemplate.h"
#include "ui_editTemplate.h"

#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

#include "config.h"
#include "style.h"

EditTemplate::EditTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editTemplate)
{
    ui->setupUi(this);

    this->setWindowTitle("Edit Template");

    this->setStyleSheet(Style::containerStyle);
    ui->browseEditButton->setStyleSheet(Style::buttonStyle);
    ui->cancelEditButton->setStyleSheet(Style::buttonStyle);
    ui->saveEditButton->setStyleSheet(Style::buttonStyle);


}

EditTemplate::~EditTemplate()
{
    delete ui;
}

void EditTemplate::on_browseEditButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", Config::riggingTemplatesPath, "All Files (*.*)");
    if (!fileName.isEmpty()) {
        ui->filePath->setText(fileName);
    }
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        ui->editEditor->setPlainText(in.readAll());
        file.close();
    }
}


void EditTemplate::on_saveEditButton_clicked()
{
    QString fileName = ui->filePath->text();
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ui->editEditor->toPlainText();
        file.close();
        this->close();

    }
}


void EditTemplate::on_cancelEditButton_clicked()
{
    this->close();
}

