#ifndef EDITFILE_H
#define EDITFILE_H

#pragma once

#include <QWidget>
#include <QApplication>
#include <QFileDialog>
#include <QTextStream>

#include "scriptEditor.h"

class EditFile : public QObject
{
    Q_OBJECT

public:
    EditFile(CodeEditor* editor, QObject *parent = nullptr);

    void newFile();
    void openFile();
    void saveFile();
    void exitApp();

private:
    CodeEditor *editor;
};

#endif // EDITFILE_H
