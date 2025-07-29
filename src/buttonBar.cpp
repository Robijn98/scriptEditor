#include "buttonBar.h"
#include "ui_buttonBar.h"
#include "style.h"

ButtonBar::ButtonBar(CodeEditor* editor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonBar)
{
    ui->setupUi(this);
    ui->saveButton->setStyleSheet(Style::iconButtonStyle);
    ui->clearButton->setStyleSheet(Style::iconButtonStyle);
    ui->runButton->setStyleSheet(Style::iconButtonStyle);
    ui->searchAndReplaceButton->setStyleSheet(Style::iconButtonStyle);
    ui->templateButton->setStyleSheet(Style::iconButtonStyle);

    editfile = new EditFile(editor);

    searchandreplace = new SearchAndReplace(editor);
    edittemplate = new EditTemplate();

}


ButtonBar::~ButtonBar()
{
    delete ui;
}


void ButtonBar::on_saveButton_clicked()
{
    editfile->saveFile();
}


void ButtonBar::on_clearButton_clicked()
{
    editfile->newFile();
}


void ButtonBar::on_searchAndReplaceButton_clicked()
{
    searchandreplace->show();
    searchandreplace->raise();
    searchandreplace->setFocus();

}

void ButtonBar::on_templateButton_clicked()
{
    edittemplate->show();
    edittemplate->raise();
    edittemplate->setFocus();
}

