#include "buttonBar.h"
#include "ui_buttonBar.h"
#include "style.h"
#include "maya/MGlobal.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "config.h"
#include "mainUserScript.h"

ButtonBar::ButtonBar(TabScriptEditor* tabEditor, Console* console, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::ButtonBar),
    tabEditor(tabEditor),
    console(console)
{
    // Initialize the UI ----------------------------------------------------------
    ui->setupUi(this);
    ui->saveButton->setStyleSheet(Style::iconButtonStyle);
    ui->clearButton->setStyleSheet(Style::iconButtonStyle);
    ui->runButton->setStyleSheet(Style::iconButtonStyle);
    ui->searchAndReplaceButton->setStyleSheet(Style::iconButtonStyle);
    ui->templateButton->setStyleSheet(Style::iconButtonStyle);
    ui->runPartialButton->setStyleSheet(Style::iconButtonStyle);
    ui->clearTerminalButton->setStyleSheet(Style::iconButtonStyle);
    ui->runMainScript->setStyleSheet(Style::iconButtonStyle);

    // Initialize the button bar ---------------------------------------------------
    editfile = new EditFile(tabEditor);

    searchandreplace = new SearchAndReplace(tabEditor, this);
    opentemplate = new OpenTemplate(tabEditor, TemplateMode::Load, this);

    //add tooltips -----------------------------------------------------------
    ui->saveButton->setToolTip("Save Script");
    ui->clearButton->setToolTip("Clear Current Editor");
    ui->runButton->setToolTip("Run Script");
    ui->searchAndReplaceButton->setToolTip("Search and Replace");
    ui->templateButton->setToolTip("Open Template");
    ui->runPartialButton->setToolTip("Run Partial Code");
    ui->clearTerminalButton->setToolTip("Clear Terminal");
    ui->runMainScript->setToolTip("Run Main Script");

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
    CodeEditor* editor = currentEditor();
    if (!editor) return;

    // Clear the current editor
    editor->clear();

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


void ButtonBar::on_runMainScript_clicked()
{

    
    // Get the main script path from the configuration
    QString mainScriptPath = Config::mainScriptPath;
    if (mainScriptPath.isEmpty()) {
        MGlobal::displayError("Main script path is not set in the configuration.");
        return;
    }

    // Load the main script into the current editor
    QFile file(mainScriptPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        MGlobal::displayError("Failed to open main script");
        return;
    }

    QTextStream in(&file);
    QString scriptContent = in.readAll();
    file.close();

    //runcode without loading it to editor
    console->runCode(scriptContent, false);
}