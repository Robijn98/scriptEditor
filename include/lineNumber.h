#ifndef LINENUMBER_H
#define LINENUMBER_H
#include "scriptEditorPanel.h"

#include <QWidget>


class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    explicit LineNumberArea(CodeEditor  *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

#endif
