#include "searchAndReplace.h"
#include "ui_searchAndReplace.h"
#include "style.h"
#include <QRegularExpression>

SearchAndReplace::SearchAndReplace(CodeEditor* editor, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchAndReplace)
    , editor(editor)
{
    ui->setupUi(this);
    this->setWindowTitle("Search And Replace");

    this->setStyleSheet(Style::containerStyle);
    ui->replaceAllButton->setStyleSheet(Style::buttonStyle);
    ui->replaceButton->setStyleSheet(Style::buttonStyle);
    ui->cancelButton->setStyleSheet(Style::buttonStyle);
    ui->findNextButton->setStyleSheet(Style::buttonStyle);
    ui->matchCaseCheckBox->setStyleSheet(Style::checkBoxStyle);

}

SearchAndReplace::~SearchAndReplace()
{
    delete ui;
}


void SearchAndReplace::on_cancelButton_clicked()
{
    this->close();
}


void SearchAndReplace::on_findNextButton_clicked()
{

    if (!editor) return;

    QString currentWord = ui->findInput->text();
    if (currentWord.isEmpty()) return;

    QTextDocument::FindFlags flags;
    if (ui->matchCaseCheckBox->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor currentCursor = editor->textCursor();

    QTextCursor foundCursor = editor->document()->find(currentWord, currentCursor, flags);

    if (foundCursor.isNull()) {
        foundCursor = editor->document()->find(currentWord, QTextCursor(editor->document()), flags);
    }

    if (!foundCursor.isNull()) {
        editor->setTextCursor(foundCursor);
    }

}


void SearchAndReplace::on_replaceButton_clicked()
{
    if (!editor) return;

    QString replaceWord = ui->replaceInput->text();

    QTextCursor currentCursor = editor->textCursor();
    currentCursor.removeSelectedText();
    currentCursor.insertText(replaceWord);
}



void SearchAndReplace::on_replaceAllButton_clicked()
{
    if (!editor) return;
    QString currentWord = ui->findInput->text();
    QString replaceWord = ui->replaceInput->text();

    QTextDocument::FindFlags flags;
    if (ui->matchCaseCheckBox->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }


    editor->moveCursor(QTextCursor::Start);

    while(editor->find(currentWord, flags))
    {
        editor->textCursor().insertText(replaceWord);
    }


}


