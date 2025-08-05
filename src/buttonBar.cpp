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
    if (!editor) {
        terminal->appendPlainText("Error: No active editor.");
        return;
    }

    QString code = editor->toPlainText();
    QStringList lines = code.split('\n');

    for (const QString& line : lines)
    {
        if (line.trimmed().isEmpty()) continue;

        MString result;
        MStatus status = MGlobal::executePythonCommand(line.toUtf8().constData(), result, true, true);

        if (status)
        {
            // Show the line and its result (if any)
            QString output = QString::fromUtf8(result.asChar());
            if (output.isEmpty())
                output = "(No output)";
            terminal->appendPlainText(QString("> %1\nResult: %2\n").arg(line).arg(output));
        }
        else
        {
            terminal->appendPlainText(QString("Error executing line: %1").arg(line));
        }
    }

}
