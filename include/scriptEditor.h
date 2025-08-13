//header file encapsulates the CodeEditor class, which is used to create a code editor with line numbers and syntax highlighting in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

#pragma once

#include <QWidget>
#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    // Constructor for the CodeEditor class
    CodeEditor(QWidget *parent = 0);
    // creates the line number area
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    // updates the line number area width based on the number of blocks
    int lineNumberAreaWidth() const;
    // sets the line number area
    QSize sizeHint() const override;



protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    // updates the line number area width when the block count changes
    void updateLineNumberAreaWidth(int newBlockCount);
    // updates the line number area when the viewport is scrolled
    void updateLineNumberArea(const QRect &, int);
    // allows for zooming in the editor
    void wheelEvent(QWheelEvent *event) override;
private:
    QWidget *lineNumberArea;
};
