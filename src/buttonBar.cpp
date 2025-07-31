#include "buttonBar.h"
#include "ui_buttonBar.h"
#include "style.h"
#include "maya/MGlobal.h"


ButtonBar::ButtonBar(CodeEditor* editor, Terminal* terminal, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ButtonBar),
      editor(editor),
      terminal(terminal)
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

void ButtonBar::on_runButton_clicked()
{
    QString code = editor->toPlainText();
    QByteArray codeUtf8 = code.toUtf8();
    MString result;
    MStatus status = MGlobal::executePythonCommand(codeUtf8.constData(), result, true, true);

    // If the code is a print statement, evaluate its argument
    if (codeUtf8.startsWith("print(")) 
    {
        // Extract the inside of print(...)
        QString insidePrint = code.mid(code.indexOf("print(") + 6);
        if (insidePrint.endsWith(")"))
            insidePrint.chop(1);

        // Construct the command to print the expression exactly as is
        QString pyCommand = QString("print(%1)").arg(insidePrint);

        MString mayaCommand(pyCommand.toUtf8().constData());
        MString printResult;


            terminal->appendPlainText(QString::fromUtf8(printResult.asChar()));
        
        else
        {
            terminal->appendPlainText("Error: Failed to execute print command.");
        }
    }

    else
    {
        if (status) 
        {
            terminal->appendPlainText("Result: " + QString::fromUtf8(result.asChar()));
        } 
        else 
        {
            MString error;
            MGlobal::executeCommand("print('Python error caught.')", error);
            terminal->appendPlainText(QString::fromUtf8(error.asChar()));
        }
    }
}
