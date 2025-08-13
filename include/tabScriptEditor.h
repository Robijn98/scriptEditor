//header file that encapsulates the TabScriptEditor class, which is used to manage multiple script editor tabs in the application
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

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
    // Constructor for the TabScriptEditor class
    explicit TabScriptEditor(QWidget *parent = nullptr);
    //manages what editor is currently active
    CodeEditor *currentEditor() const;
    //allows for a split to occure in the editors so you can view multiple editors at once
    void splitEditor();
    //handles how tabs are closed and prevents for the last one to be closed
    void closeTab(QTabWidget* tabWidget);
    //creates a new tab in the tab widget
    void newTab(QTabWidget* tw = nullptr);
    //closes all but the last tab
    void closeAllTabs();
    //allows for the renaming of a tab
    void renameTab();
    //saves the current state of the tab editor
    void saveState();
    //loads the saved state of the tab editor
    void loadState();
    //resets the layout to pre-set
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
    // Internal function to close all tabs without emitting signals
    void closeAllTabsInternal();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

};

#endif // TABSCRIPTEDITOR_H
