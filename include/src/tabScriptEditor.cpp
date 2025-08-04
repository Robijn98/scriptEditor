#include "tabScriptEditor.h"
#include <QVBoxLayout>
#include <QTabBar>
#include <QSplitter>
#include "style.h"
#include <iostream>
#include <QApplication>

TabScriptEditor::TabScriptEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    splitter = new QSplitter(Qt::Horizontal, this);
    layout->addWidget(splitter);

    QTabWidget* tw = createTabWidget();
    splitter->addWidget(tw);
    // focusedTabWidget = tw;

    this->setStyleSheet(Style::tabStyle);
}





void TabScriptEditor::closeTab()
{
    QTabWidget* editor = getFocusedTabWidget();
    if (!editor) return;

    if (editor->count() <= 1) {
        std::cout << "Cannot close the last tab." << std::endl;
        return; 
    }

    editor->clearFocus();
    if (QWidget* focused = QApplication::focusWidget()) {
        focused->clearFocus();
        QApplication::setActiveWindow(nullptr);
    }
    

    int index = splitter->indexOf(editor);
    if (index != -1)
    {
        QWidget* w = splitter->widget(index);
        w->setParent(nullptr);
    }

    tabWidgets.removeAll(editor);
    editor->deleteLater();
}



void TabScriptEditor::newTab(QTabWidget* tw) {
    if (!tw)
        tw = getFocusedTabWidget(); 

    if (!tw)
        return;

    // Remove "+" tab temporarily (assumes it's the last one)
    int plusIndex = tw->count() - 1;
    if (tw->tabText(plusIndex) == "+") {
        QWidget* plus = tw->widget(plusIndex);
        tw->removeTab(plusIndex);
        delete plus; // clean up the old placeholder
    }

    // Create actual editor
    CodeEditor *editor = new CodeEditor();
    editor->installEventFilter(this); 
    new Highlighter(editor->document());

    tw->addTab(editor, tr("py"));

    // Re-add new "+" tab
    QWidget* newPlusTab = new QWidget();
    tw->addTab(newPlusTab, "+");
    tw->tabBar()->setTabButton(tw->count() - 1, QTabBar::RightSide, nullptr);

    tw->setCurrentWidget(editor);
}



CodeEditor* TabScriptEditor::currentEditor() const
{
    QTabWidget* current = getFocusedTabWidget();
    if (!current)
        return nullptr;

    return qobject_cast<CodeEditor*>(current->currentWidget());
}




QTabWidget* TabScriptEditor::createTabWidget()
{
    QTabWidget* tw = new QTabWidget(this);
    tabWidgets.append(tw);
    tw->setTabsClosable(true);

    newTab(tw);


connect(tw, &QTabWidget::tabCloseRequested, this, [=](int index) {
    if (tw->tabText(index) == "+") {
        return; // don't allow closing the "+" tab
    }

    QWidget *w = tw->widget(index);
    tw->removeTab(index);
    w->deleteLater();
    else if (tw->count() > 0) 
    {
        std::cout << "Closing last user tab, removing tab widget." << std::endl;
        this->closeTab();
    }
    //  {
    //     // Check if only the "+" tab remains after removal
    //     if (tw->count() == 1 && tw->tabText(0) == "+") {
    //         std::cout << "Closing last user tab, removing tab widget." << std::endl;
    //         this->closeTab();
    //     }
    // }

});



    
    connect(tw, &QTabWidget::currentChanged, this, [=](int index) {
        if (tw->tabText(index) == "+") {
            newTab(tw);
        }
    });

    int plusTabIndex = tw->count() - 1;
    tw->tabBar()->setTabButton(plusTabIndex, QTabBar::RightSide, nullptr);

    tw->setMovable(true);
    tw->setAcceptDrops(true);
    tw->tabBar()->installEventFilter(this);


    return tw;
}



void TabScriptEditor::splitEditor()
{
    QTabWidget* newTabView = createTabWidget();
    splitter->addWidget(newTabView);
    splitter->setStretchFactor(splitter->count() - 1, 1);
}



QTabWidget* TabScriptEditor::getFocusedTabWidget() const
{
    if (focusedTabWidget && tabWidgets.contains(focusedTabWidget))
        return focusedTabWidget;

    return tabWidgets.isEmpty() ? nullptr : tabWidgets.first();
}



bool TabScriptEditor::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::FocusIn) {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (!widget) return false;

        for (QTabWidget* tw : tabWidgets) {
            int index = tw->indexOf(widget);
            if (index != -1) {
                focusedTabWidget = tw;
                break;
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}
