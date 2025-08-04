#include "buttonBar.h"
#include "ui_buttonBar.h"
#include "style.h"
#include "maya/MGlobal.h"


ButtonBar::ButtonBar(TabScriptEditor* tabEditor, Terminal* terminal, QWidget* parent)
    : QWidget(parent),
    ui(new Ui::ButtonBar),
    tabEditor(tabEditor),
    terminal(terminal)
{
    ui->setupUi(this);
    ui->saveButton->setStyleSheet(Style::iconButtonStyle);
    ui->clearButton->setStyleSheet(Style::iconButtonStyle);
    ui->runButton->setStyleSheet(Style::iconButtonStyle);
    ui->searchAndReplaceButton->setStyleSheet(Style::iconButtonStyle);
    ui->templateButton->setStyleSheet(Style::iconButtonStyle);

    editfile = new EditFile(tabEditor);

    searchandreplace = new SearchAndReplace(tabEditor->currentEditor());
    edittemplate = new EditTemplate();

}


ButtonBar::~ButtonBar()
{
    delete ui;
}

CodeEditor* ButtonBar::currentEditor() const
{
    return qobject_cast<CodeEditor*>(tabWidget->currentWidget());
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
    CodeEditor* editor = currentEditor();
    if (!editor) {
        terminal->appendPlainText("Error: No active editor.");
        return;
    }

    QString code = editor->toPlainText();
    QByteArray codeUtf8 = code.toUtf8();
    MString result;
    MStatus status = MGlobal::executePythonCommand(codeUtf8.constData(), result, true, true);

    if (codeUtf8.startsWith("print("))
    {
        QString insidePrint = code.mid(code.indexOf("print(") + 6);
        if (insidePrint.endsWith(")"))
            insidePrint.chop(1);

        QString pyCommand = QString("print(%1)").arg(insidePrint);
        MString mayaCommand(pyCommand.toUtf8().constData());
        MString printResult;
        MStatus printStatus = MGlobal::executePythonCommand(mayaCommand, printResult, true, true);

        if (printStatus) {
            terminal->appendPlainText(QString::fromUtf8(printResult.asChar()));
        } else {
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
