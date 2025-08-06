#include "console.h"
#include <QThread>
#include <QDir>
#include <QTextStream>
#include <maya/MGlobal.h>
#include <QFont>


Console::Console(QWidget *parent) : QPlainTextEdit(parent)
{
    
    QPlainTextEdit::setReadOnly(true);
    QPlainTextEdit::setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    QFont font = this->font();
    font.setFamily("Courier New");
    font.setPointSize(10);
    this->setFont(font);
}


void Console::runCode(CodeEditor *editor, bool runPartial)
{
    if (!editor) {
        appendPlainText("Error: No active editor");
        return;
    }

    QString code;
    if (runPartial) {
        code = editor->textCursor().selectedText();
        if (code.isEmpty()) {
            appendHtml("<span style='color:red;'>Error: No text selected</span>");
            return;
        }
    } else {
        code = editor->toPlainText();
        if (code.isEmpty()) {
            appendHtml("<span style='color:red;'>Error: No code to run</span>");
            return;
        }
    }

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
)").arg(pyCode).arg(outFile).arg(errFile).arg(retFile);

        MStatus status = MGlobal::executePythonCommand(pyWrapper.toUtf8().constData(), false, true);

        appendPlainText("> " + line);

        if (!status) {
            appendHtml("<span style='color:red;'>Error executing line.</span>");
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

            // Cleanup
            QFile::remove(outFile);
            QFile::remove(errFile);
            QFile::remove(retFile);
        }

        ++counter;
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