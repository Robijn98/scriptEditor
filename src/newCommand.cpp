#include "newCommand.h"
#include "ui_newCommand.h"
#include "config.h"
#include "style.h"

#include <QRegularExpression>
#include <QInputDialog>
#include <QTextStream>
#include <QFileDialog>
#include <qmessagebox.h>

NewCommand::NewCommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCommand)
{
    ui->setupUi(this);

    if(!ui->classCheckBox->isChecked())
    {
        ui->functionEdit->hide();
        ui->functionLabel->hide();
    }
    connect(ui->classCheckBox, &QCheckBox::toggled, this, &NewCommand::on_classCheckBox_toggled);

    //style settings
    this->setWindowTitle("New rigging command");

    this->setStyleSheet(Style::containerStyle);

    ui->cancelButton->setStyleSheet(Style::buttonStyle);

    ui->saveButton->setStyleSheet(Style::buttonStyle);

    ui->classCheckBox->setStyleSheet(Style::checkBoxStyle);
}

NewCommand::~NewCommand()
{
    delete ui;
}


void NewCommand::on_saveButton_clicked()
{
    // Get the file name from the line edit
    QString fileNamed = ui->fileNameLine->text().trimmed();
    if (fileNamed.isEmpty()) {
        QMessageBox::warning(this, "Invalid Filename", "Please enter a file name");
        return;
    }

    QString baseDir = Config::riggingCommandsPath;
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
    QString code = ui->editor->toPlainText();

    // Append class/function info if checkbox is checked
    if (ui->classCheckBox->isChecked()) {
        QString className;
        QRegularExpression classDef(R"(\bclass\s+(\w+))");
        QRegularExpressionMatch match = classDef.match(code);

        if (match.hasMatch()) {
            className = match.captured(1);
            className += ".";
            className += ui->functionEdit->text();
            code += "\n#CLASS AND FUNCTION";
            code += "\n#" + className;
        } else {
            QMessageBox::warning(this, "No Class Found", "Class checkbox is checked but no class found in code.");
        }
    }

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




void NewCommand::on_cancelButton_clicked()
{
    this->close();
}


void NewCommand::on_classCheckBox_toggled(bool checked)
{
    ui->functionEdit->setVisible(checked);
    ui->functionLabel->setVisible(checked);
}

