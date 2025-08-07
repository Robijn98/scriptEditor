#include "newTemplate.h"
#include "ui_newTemplate.h"
#include "config.h"
#include "style.h"

#include <QTextStream>
#include <QMessageBox>
#include <QFile>

NewTemplate::NewTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTemplate)
{
    ui->setupUi(this);

    this->setWindowTitle("New template");
    this->setStyleSheet(Style::containerStyle);
    ui->cancelTemplateButton->setStyleSheet(Style::buttonStyle);
    ui->saveTemplateButton->setStyleSheet(Style::buttonStyle);

}

NewTemplate::~NewTemplate()
{
    delete ui;
}

void NewTemplate::on_saveTemplateButton_clicked()
{
    QString fileNamed = ui->fileName->text().trimmed();
    if (fileNamed.isEmpty()) {
        QMessageBox::warning(this, "Invalid Filename", "Please enter a file name");
        return;
    }

    QString baseDir = Config::riggingTemplatesPath;
    QString filePath = baseDir + "/" +fileNamed;

    // Check if the file already exists
    if (QFile::exists(filePath)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "File Exists",
                                      "A file with that name already exists. Overwrite?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes)
            return;
    }


    // Get code from editor
    QString code = ui->templateEditor->toPlainText();
    // Write to the file
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << code;
        file.close();
        QMessageBox::information(this, "Saved", "File saved successfully to:\n" + filePath);
    } else {
        QMessageBox::critical(this, "Error", "Could not open file for writing:\n" + filePath);
    }

    this->close();
}


void NewTemplate::on_cancelTemplateButton_clicked()
{
    this->close();
}

