#include "openTemplate.h"
#include "ui_openTemplate.h"

#include "style.h"
#include "config.h"

#include <QTextStream>
#include <QMessageBox>
#include <QDebug>


OpenTemplate::OpenTemplate(CodeEditor* editor, TemplateMode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenTemplate),
    editor(editor)
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


//read folder content into list, reuse from commandList
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
    QString templateString;
    QString completeFileName = QString("%1/%2.py").arg(dir.path()).arg(fileName);

    if (completeFileName.isEmpty()) throw std::invalid_argument( "please provide valid filename");

    QFile file(completeFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        editor->setPlainText(in.readAll());
        file.close();
    }
}

void OpenTemplate::on_loadButton_clicked()
{
    //qDebug() << "Clicked in mode:" << (mode == TemplateMode::Load ? "Load" : "Remove");
    if(this->ui->templateList->selectedItems().size() == 0)
    {
        QMessageBox::warning(this, "Invalid Filename", "Please enter a file name");
        return;
    }

    QListWidgetItem *selectedFile = ui->templateList->currentItem();
    QString fileName = selectedFile->text();

    QDir dir(Config::riggingTemplatesPath);
    QString completeFileName = QString("%1/%2.py").arg(dir.path()).arg(fileName);

        if (mode == TemplateMode::Load)
        {
            loadTemplate(fileName, dir);
            this->close();
        }

        else if (mode == TemplateMode::Remove)
        {
            QFile::remove(completeFileName);
            loadList();
        }


}


void OpenTemplate::on_cancelButton_clicked()
{
    this->close();
}


void OpenTemplate::loadList()
{
    while(ui->templateList->count()>0)
    {
        ui->templateList->clear();
    }
    std::list<QString> templateList;
    QDir dir(Config::riggingTemplatesPath);

    templateList = convertToList(dir);
    populateList(templateList);
}


