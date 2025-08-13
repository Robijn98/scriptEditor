//header file encapsulates the LineNumberArea class, which is used to display line numbers in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef LINENUMBER_H
#define LINENUMBER_H
#include "scriptEditorPanel.h"

#include <QWidget>


class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    // Constructor for the LineNumberArea class
    explicit LineNumberArea(CodeEditor  *editor);
    // Returns the size hint for the line number area
    QSize sizeHint() const override;

protected:
    // Paint event for the line number area, creates the line numbers
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

#endif
