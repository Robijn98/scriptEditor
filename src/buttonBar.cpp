#include "buttonBar.h"
#include "ui_buttonBar.h"
#include "style.h"
#include "maya/MGlobal.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDir>


ButtonBar::ButtonBar(TabScriptEditor* tabEditor, Console* console, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::ButtonBar),
    tabEditor(tabEditor),
    console(console)
{
    ui->setupUi(this);
    ui->saveButton->setStyleSheet(Style::iconButtonStyle);
    ui->clearButton->setStyleSheet(Style::iconButtonStyle);
    ui->runButton->setStyleSheet(Style::iconButtonStyle);
    ui->searchAndReplaceButton->setStyleSheet(Style::iconButtonStyle);
    ui->templateButton->setStyleSheet(Style::iconButtonStyle);

    editfile = new EditFile(tabEditor);

    searchandreplace = new SearchAndReplace(tabEditor->currentEditor());
    opentemplate = new OpenTemplate(tabEditor, TemplateMode::Load, this);

}


ButtonBar::~ButtonBar()
{
    delete ui;
}
CodeEditor* ButtonBar::currentEditor() const
{
    if (!tabEditor) return nullptr;
    return tabEditor->currentEditor();
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
{
    CodeEditor* editor = currentEditor();
    if (!editor) return;

    OpenTemplate* opentemplate = new OpenTemplate(tabEditor, TemplateMode::Load, this);
    opentemplate->loadList();
    opentemplate->show();
    opentemplate->raise();
    opentemplate->setFocus();
}
}


void ButtonBar::on_runButton_clicked()
{
    CodeEditor* editor = currentEditor();
    console->runCode(currentEditor(), false);
}

void ButtonBar::on_runPartialButton_clicked()
{
    CodeEditor* editor = currentEditor();
    console->runCode(currentEditor(), true);
}


void ButtonBar::on_clearTerminalButton_clicked()
{
    if (console) {
        console->clear();  
    }
}


