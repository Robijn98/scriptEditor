#include "mainUserScript.h"
#include "ui_mainScript.h"
#include "config.h"
#include "style.h"

#include <QTextStream>
#include <QMessageBox>
#include <QFile>

MainUserScript::MainUserScript(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveMainScript)
{
    ui->setupUi(this);
    this->setWindowTitle("Save Main Script");
    this->setStyleSheet(Style::containerStyle);
    ui->cancelMainScriptButton->setStyleSheet(Style::buttonStyle);
    ui->saveMainScriptButton->setStyleSheet(Style::buttonStyle);
}

MainUserScript::~MainUserScript()
{
    delete ui;
}


void MainUserScript::on_saveMainScriptButton_clicked()
{
    QString fileNamed = ui->fileName->text().trimmed();
    if (fileNamed.isEmpty()) {
        QMessageBox::warning(this, "Invalid Filename", "Please enter a file name");
        return;
    }

    QString baseDir = Config::mainScriptPath;
    QString filePath = baseDir;



    // Get code from editor
    QString code = ui->mainScriptEditor->toPlainText();
    // Write to the file
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << code;
        file.close();
        QMessageBox::information(this, "Saved", "File saved successfully to:\n" + filePath);
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Could not open file for writing:\n" + filePath);
    }
}

void MainUserScript::on_cancelMainScriptButton_clicked()
{
    this->close();
}