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


