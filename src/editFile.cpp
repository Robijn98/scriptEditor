#include "editFile.h"

EditFile::EditFile(CodeEditor* editor, QObject *parent)
    : QObject(parent), editor(editor)
{

}

void EditFile::exitApp()
{
    QApplication::quit();
}

void EditFile::newFile()
{
    editor->clear();
}

void EditFile::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(editor, "Open File");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        editor->setPlainText(in.readAll());
        file.close();
    }
}

void EditFile::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(editor, "Save File");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << editor->toPlainText();
        file.close();
    }
}
