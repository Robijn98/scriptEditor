//header file for the Console class
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

#pragma once

#include <QWidget>
#include <QPlainTextEdit>
#include "scriptEditor.h"


class Console : public QPlainTextEdit
{
    Q_OBJECT    

    public:
        // Constructor for the Console class
        Console(QWidget *parent = nullptr);
        // runs the code in the current editor
        void runCode(CodeEditor *editor, bool runPartial = false);
        void runCode(const QString &code, bool runPartial = false);
    private:
        CodeEditor* currentEditor() const;
        // makes the console zoomable
        void wheelEvent(QWheelEvent *event) override;

};