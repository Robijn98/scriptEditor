#include "openTemplate.h"
#include "ui_openTemplate.h"

#include "style.h"
#include "config.h"

#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <iostream>


OpenTemplate::OpenTemplate(TabScriptEditor* tabEditor, TemplateMode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenTemplate),
    tabEditor(tabEditor)
{
    ui->setupUi(this);
    this->mode = mode;

    this->setWindowTitle(mode == TemplateMode::Load ? "Open Template" : "Remove Template");
    this->setStyleSheet(Style::containerStyle);
    ui->templateList->setStyleSheet(Style::listStyle);
    ui->loadButton->setText(mode == TemplateMode::Load ? "Load Template" : "Remove Template");
    ui->loadButton->setStyleSheet(Style::buttonStyle);
    ui->cancelButton->setStyleSheet(Style::buttonStyle);


}

OpenTemplate::~OpenTemplate()
{
    delete ui;
}

CodeEditor* OpenTemplate::currentEditor() const
{
    if (!tabEditor) return nullptr;
    return tabEditor->currentEditor();
}

std::list<QString> OpenTemplate::convertToList(QDir dir)
{
    std::list<QString> listOut;

    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        QString name = fileInfo.fileName();

        if(name.endsWith(".py", Qt::CaseInsensitive))
        {
            name.chop(3);
            qDebug() << "Adding template: " << name;
        }
        else
        {
            qDebug() << "Skipping file: " << name;
            continue; 
        }

        listOut.push_back(name);
    }


    return listOut;
}


void OpenTemplate::populateList(std::list<QString> templateList)
{

    for(const QString &item : templateList)
    {
        ui->templateList->addItem(item);
    }
}


void OpenTemplate::loadTemplate(QString fileName, QDir dir)
{
    CodeEditor* editor = tabEditor->currentEditor();
    if (!editor) {
        std::cerr << "ERROR: currentEditor() returned nullptr" << std::endl;
        return;
    } else {
        std::cout << "Current editor is valid." << std::endl;
    }
    
    QString completeFileName = QString("%1/%2").arg(dir.path()).arg(fileName);
    if (completeFileName.isEmpty()) throw std::invalid_argument( "please provide valid filename");

    QFile file(completeFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString content = in.readAll();
        std::cout << "Template content length: " << content.size() << std::endl;
        
        editor->setPlainText(content);
        editor->show();
        editor->setFocus();

        file.close();
    } else {
        std::cerr << "ERROR: Could not open template file." << std::endl;
    }
}


void OpenTemplate::on_loadButton_clicked()
{


    if(this->ui->templateList->selectedItems().size() == 0)
    {
        QMessageBox::warning(this, "Invalid Filename", "Please enter a file name");
        return;
    }

    QListWidgetItem *selectedFile = ui->templateList->currentItem();
    QString fileName = selectedFile->text();

    QDir dir(Config::riggingTemplatesPath);
    if (!dir.exists())
    {
        QMessageBox::warning(this, "Directory Error", "The templates directory does not exist.");
        return;
    }

        // Ensure the file name is valid
        if (fileName.isEmpty() || fileName.contains(QRegExp("[^a-zA-Z0-9_]")))
        {
            QMessageBox::warning(this, "Invalid Filename", "Please enter a valid file name");
            return;
        }

    // Construct the complete file name
    if (!fileName.endsWith(".py", Qt::CaseInsensitive))
    {
        fileName.append(".py");
    }
    // QString completeFileName = QString("%1/%2.py").arg(dir.path()).arg(fileName);

        if (mode == TemplateMode::Load)
        {
            loadTemplate(fileName, dir);
            this->close();
        }

        else if (mode == TemplateMode::Remove)
        {
            QFile::remove(fileName);
            loadList();
        }


}


void OpenTemplate::on_cancelButton_clicked()
{
    this->close();
}


void OpenTemplate::loadList()
{
    ui->templateList->clear();  // Just clear once

    QDir dir(Config::riggingTemplatesPath);
    qDebug() << "Templates path:" << Config::riggingTemplatesPath;
    qDebug() << "Directory exists:" << dir.exists();

    if (!dir.exists()) {
        QMessageBox::warning(this, "Directory Error", "The templates directory does not exist.");
        return;
    }

    std::list<QString> templateList = convertToList(dir);
    populateList(templateList);
}
