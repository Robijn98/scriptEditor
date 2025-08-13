//header file for functions attatched to the file menu in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef EDITFILE_H
#define EDITFILE_H

#pragma once

#include <QWidget>
#include <QApplication>
#include <QFileDialog>
#include <QTextStream>

#include "tabScriptEditor.h"

class EditFile : public QObject
{
    Q_OBJECT

public:
    // Constructor for the EditFile class
    EditFile(TabScriptEditor* tabEditor,  QObject *parent = nullptr);

    //clears the current editor
    void newFile();
    //opens a file dialog to select a file to open
    void openFile();
    //saves the current editor content to a file
    void saveFile();
    //exit whole Bes Editor
    void exitApp();

private:
    TabScriptEditor* tabEditor;
};

#endif // EDITFILE_H
