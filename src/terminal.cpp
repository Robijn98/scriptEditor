#include "terminal.h"
#include <QThread>

Terminal::Terminal(QWidget *parent) : QPlainTextEdit(parent)
{
    
    QPlainTextEdit::setReadOnly(true);
    QPlainTextEdit::setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    return;
}

void Terminal::printPythonOutput(const QString &output)
{
    if (QThread::currentThread() == this->thread()) {
        // We are on the GUI thread, safe to call directly
        this->appendPlainText(output);
    } else {
        // Called from a different thread, queue it
        QMetaObject::invokeMethod(this, "appendPlainText",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, output));
    }
}


void Terminal::returnMayaOutput(const QString &output)
{
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
