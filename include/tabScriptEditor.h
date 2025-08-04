#ifndef TABSCRIPTEDITOR_H
#define TABSCRIPTEDITOR_H

#include <QWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QPoint>

#include "scriptEditor.h"
#include "highlighter.h"

class TabScriptEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TabScriptEditor(QWidget *parent = nullptr);
    CodeEditor *currentEditor() const;
    void splitEditor();

public slots:
    void newTab();


private:
    QTabWidget *tabWidget;
    Highlighter *highlighter;
    QSplitter *splitter;
    void newTab(QTabWidget* target);
    QTabWidget* createTabWidget();
    QTabWidget* focusedTabWidget = nullptr;
    QTabWidget* getFocusedTabWidget() const;
    QList<QTabWidget*> tabWidgets;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

};

#endif // TABSCRIPTEDITOR_H
