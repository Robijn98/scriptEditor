#pragma once

#include <QWidget>
#include <QPlainTextEdit>

class Terminal : public QPlainTextEdit
{
    Q_OBJECT    

    public:
        Terminal(QWidget *parent = nullptr);


};