#include "buttonBar.h"
#include "ui_buttonBar.h"
#include "style.h"
#include "maya/MGlobal.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDir>


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
        terminal->appendPlainText("Error: No active editor");
        return;
    }

    QString code = editor->toPlainText();
    QStringList lines = code.split('\n');

    QString tempDir = QDir::tempPath();
    int counter = 0;

    for (const QString& line : lines)
    {
        QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty()) continue;

        QString pyCode = trimmedLine;
        pyCode.replace("\\", "\\\\");
        pyCode.replace("\"", "\\\"");
        pyCode.replace("\n", "\\n");

        QString outFile = QString("%1/maya_plugin_out_%2.txt").arg(tempDir).arg(counter);
        QString errFile = QString("%1/maya_plugin_err_%2.txt").arg(tempDir).arg(counter);
        QString retFile = QString("%1/maya_plugin_ret_%2.txt").arg(tempDir).arg(counter);

        QString pyWrapper = QString(R"(import sys
from io import StringIO

_stdout = sys.stdout
_stderr = sys.stderr
sys.stdout = StringIO()
sys.stderr = StringIO()

_retval = None

try:
    _retval = eval("%1")
except SyntaxError:
    try:
        exec("%1")
    except Exception:
        import traceback
        traceback.print_exc()
except Exception:
    import traceback
    traceback.print_exc()

out = sys.stdout.getvalue()
err = sys.stderr.getvalue()

sys.stdout = _stdout
sys.stderr = _stderr

with open(r"%2", "w", encoding="utf-8") as f_out:
    f_out.write(out)

with open(r"%3", "w", encoding="utf-8") as f_err:
    f_err.write(err)

with open(r"%4", "w", encoding="utf-8") as f_ret:
    if _retval is not None:
        f_ret.write(str(_retval))
    else:
        f_ret.write("")
)").arg(pyCode).arg(outFile).arg(errFile).arg(retFile);

        // Run Python code in Maya
        MStatus status = MGlobal::executePythonCommand(pyWrapper.toUtf8().constData(), false, true);

        terminal->appendPlainText("> " + line);

        if (!status) {
            terminal->appendHtml("<span style='color:red;'>Error executing line.</span>");
        } else {
            // Read stdout
            QFile fOut(outFile);
            if (fOut.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&fOut);
                QString stdoutText = in.readAll();
                fOut.close();
                if (!stdoutText.isEmpty())
                    terminal->appendPlainText(stdoutText.trimmed());
            }

            // Read stderr
            QFile fErr(errFile);
            if (fErr.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&fErr);
                QString stderrText = in.readAll();
                fErr.close();
                if (!stderrText.isEmpty())
                    terminal->appendHtml("<span style='color:red;'>" + stderrText.trimmed() + "</span>");
            }

            // Read return value
            QFile fRet(retFile);
            if (fRet.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&fRet);
                QString retText = in.readAll();
                fRet.close();
                if (!retText.isEmpty())
                    terminal->appendHtml("<span style='color:green;'>Return: " + retText.trimmed() + "</span>");
            }

            // Remove temp files
            QFile::remove(outFile);
            QFile::remove(errFile);
            QFile::remove(retFile);
        }

        ++counter;
    }
}
