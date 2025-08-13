#include "console.h"
#include <QThread>
#include <QDir>
#include <QTextStream>
#include <maya/MGlobal.h>
#include <QFont>


Console::Console(QWidget *parent) : QPlainTextEdit(parent)
{
    //set up console UI    
    QPlainTextEdit::setReadOnly(true);
    QPlainTextEdit::setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    QFont font = this->font();
    font.setFamily("Courier New");
    font.setPointSize(10);
    this->setFont(font);
}


void Console::runCode(CodeEditor *editor, bool runPartial)
{
    // if no current editor exit function
    if (!editor) {
        appendPlainText("Error: No active editor");
        return;
    }
    // check if running partial if any text is selected, otherwise give error
    QString code;
    if (runPartial) {
        code = editor->textCursor().selectedText();
        if (code.isEmpty()) {
            appendHtml("<span style='color:red;'>Error: No text selected</span>");
            return;
        }
    }
    //check if theres any code at all
    else 
    {
        code = editor->toPlainText();
        if (code.isEmpty()) {
            appendHtml("<span style='color:red;'>Error: No code to run</span>");
            return;
        }
    }
    // replace paragraph separator with normal newline
    code = code.replace(QChar(0x2029), QChar('\n'));  


    // Show the entire code block at once
    appendPlainText("> " + code);

    QString tempDir = QDir::tempPath();

    QString outFile = QString("%1/maya_plugin_out.txt").arg(tempDir);
    QString errFile = QString("%1/maya_plugin_err.txt").arg(tempDir);
    QString retFile = QString("%1/maya_plugin_ret.txt").arg(tempDir);

    // Escape backslashes and quotes and newlines for the Python string
QString pyCode = code;
pyCode.replace("'''", "\\'\\'\\'");  // escape triple single quotes in user code

QString pyWrapper = QString(R"(
import sys
from io import StringIO
import traceback

_stdout = sys.stdout
_stderr = sys.stderr
sys.stdout = StringIO()
sys.stderr = StringIO()

_retval = None

code = '''%1'''

try:
    _retval = eval(code)
except SyntaxError:
    try:
        exec(code)
    except Exception:
        traceback.print_exc()
except Exception:
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
)").arg(pyCode).arg(outFile).arg(errFile).arg(retFile);

    MStatus status = MGlobal::executePythonCommand(pyWrapper.toUtf8().constData(), false, true);

    if (!status) {
        appendHtml("<span style='color:white;'>Error executing code.</span>");
    } else {
        // Read stdout
        QFile fOut(outFile);
        if (fOut.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&fOut);
            QString stdoutText = in.readAll();
            fOut.close();
            if (!stdoutText.isEmpty())
                appendPlainText(stdoutText.trimmed());
        }

        // Read stderr
        QFile fErr(errFile);
        if (fErr.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&fErr);
            QString stderrText = in.readAll();
            fErr.close();
            if (!stderrText.isEmpty())
                appendHtml("<span style='color:red;'>" + stderrText.trimmed() + "</span>");
        }

        // Read return value
        QFile fRet(retFile);
        if (fRet.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&fRet);
            QString retText = in.readAll();
            fRet.close();
            if (!retText.isEmpty())
                appendHtml("<span style='color:green;'>Return: " + retText.trimmed() + "</span>");
        }

        // Cleanup temp files
        QFile::remove(outFile);
        QFile::remove(errFile);
        QFile::remove(retFile);
    }
}



void Console::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    } else {
        QPlainTextEdit::wheelEvent(event);
    }
}