#include "tabScriptEditor.h"
#include <QVBoxLayout>
#include <QTabBar>
#include <QSplitter>
#include "style.h"
#include <iostream>
#include <QApplication>
#include <QInputDialog>

TabScriptEditor::TabScriptEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);


    splitter = new QSplitter(Qt::Horizontal, this);
    layout->addWidget(splitter);

    QTabWidget* tw = createTabWidget();
    splitter->addWidget(tw);
   
    this->setStyleSheet(Style::tabStyle);
    this->updateGeometry();
    this->adjustSize();
    this->update();
}




void TabScriptEditor::closeTab(QTabWidget* tabWidget)
{
    if(!tabWidget) return;

    if (tabWidgets.size() <= 1) 
    {
        std::cout << "Cannot close the last editor widget." << std::endl;
        return;
    }

    tabWidget->clearFocus();
    if (QWidget* focused = QApplication::focusWidget()) {
        focused->clearFocus();
    }
    

    int index = splitter->indexOf(tabWidget);
    if (index != -1)
    {
        QWidget* w = splitter->widget(index);
        w->setParent(nullptr);
    }

    tabWidgets.removeAll(tabWidget);
    if(focusedTabWidget == tabWidget) {
        focusedTabWidget = nullptr; 
    }
    tabWidget->deleteLater();

    // restore focus to another tab widget
    QTabWidget* fallbackTab = getFocusedTabWidget();
    if (fallbackTab && fallbackTab->currentWidget()) {
        fallbackTab->currentWidget()->setFocus(Qt::OtherFocusReason);
    }
    
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
    // if (tw->tabText(index) == "+") {
    //     return;
    // }

    QWidget* w = tw->widget(index);


    // Count how many tabs are user tabs, not including plus
    int userTabsCount = 0;
    for (int i = 0; i < tw->count(); ++i) {
        if (tw->tabText(i) != "+") {
            ++userTabsCount;
        }
    }

    tw->removeTab(index);
    w->deleteLater();

    // If no user tabs remain, close the whole tab widget
    if (userTabsCount == 1) {
        std::cout << "No user tabs left, removing tab widget." << std::endl;
        this->closeTab(tw);
    }
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


void TabScriptEditor::closeAllTabs()

{
    for (QTabWidget* tw : tabWidgets) {
        while (tw->count() > 1) {
            int index = tw->count() - 1;
            if (tw->tabText(index) == "+") {
                tw->removeTab(index);
                continue;
            }
            QWidget* w = tw->widget(index);
            tw->removeTab(index);
            w->deleteLater();
        }
    }

    tabWidgets.clear();
    focusedTabWidget = nullptr;
}


void TabScriptEditor::renameTab()
{
    QTabWidget* tw = getFocusedTabWidget();
    if (!tw) return;

    int index = tw->currentIndex();
    if (index < 0 || index >= tw->count()) return;

    QString newName = QInputDialog::getText(this, "Rename Tab", "Enter new tab name:");
    if (newName.isEmpty()) return;

    tw->setTabText(index, newName);
    tw->setTabToolTip(index, newName); 
    
}