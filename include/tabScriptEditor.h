#ifndef TABSCRIPTEDITOR_H
#define TABSCRIPTEDITOR_H

#include <QWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QPoint>
#include <QSettings>

#include "scriptEditor.h"
#include "highlighter.h"

class TabScriptEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TabScriptEditor(QWidget *parent = nullptr);
    CodeEditor *currentEditor() const;
    void splitEditor();
    void closeTab(QTabWidget* tabWidget);
    void newTab(QTabWidget* tw = nullptr);
    void closeAllTabs();
    void renameTab();
    void saveState();
    void loadState();
    void resetLayout();

signals:
    void nameChanged();
    void allTabsClosed();

private:
    QTabWidget *tabWidget;
    Highlighter *highlighter;
    QSplitter *splitter;
    QTabWidget* createTabWidget(bool withDefaultTab = true);
    QTabWidget* focusedTabWidget = nullptr;
    QTabWidget* getFocusedTabWidget() const;
    QList<QTabWidget*> tabWidgets;
    void closeAllTabsInternal();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

};

#endif // TABSCRIPTEDITOR_H
