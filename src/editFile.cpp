#include "editFile.h"


EditFile::EditFile(TabScriptEditor* tabEditor, QObject* parent)
    : QObject(parent), tabEditor(tabEditor) {}

void EditFile::newFile() {
    tabEditor->newTab();
}

void EditFile::openFile() {
    CodeEditor* editor = tabEditor->currentEditor();
    if (!editor) return;

    QString fileName = QFileDialog::getOpenFileName(editor, "Open File");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        editor->setPlainText(in.readAll());
        file.close();
    }
}

void EditFile::saveFile() {
    CodeEditor* editor = tabEditor->currentEditor();
    if (!editor) return;

    QString fileName = QFileDialog::getSaveFileName(editor, "Save File");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << editor->toPlainText();
        file.close();
    }
}

void EditFile::exitApp() {
    QApplication::quit();
}
