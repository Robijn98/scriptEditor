#include "terminal.h"
#include <QThread>

Terminal::Terminal(QWidget *parent) : QPlainTextEdit(parent)
{
    
    QPlainTextEdit::setReadOnly(true);
    QPlainTextEdit::setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    return;
}
