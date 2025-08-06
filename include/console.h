#pragma once

#include <QWidget>
#include <QPlainTextEdit>

#include "scriptEditor.h"


class Console : public QPlainTextEdit
{
    Q_OBJECT    

    public:
        Console(QWidget *parent = nullptr);
        void runCode(CodeEditor *editor, bool runPartial = false);

    private:
        CodeEditor* currentEditor() const;
        void wheelEvent(QWheelEvent *event) override;

};