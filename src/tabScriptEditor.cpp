#include "tabScriptEditor.h"
#include <QVBoxLayout>
#include <QTabBar>
#include <QSplitter>
#include "style.h"
#include <iostream>
#include <QApplication>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDockWidget>
#include <QMainWindow>


TabScriptEditor::TabScriptEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);


    splitter = new QSplitter(Qt::Horizontal, this);
    layout->addWidget(splitter);

    QTabWidget* tw = createTabWidget(true); 
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




QTabWidget* TabScriptEditor::createTabWidget(bool withDefaultTab)
{
    QTabWidget* tw = new QTabWidget(this);
    tabWidgets.append(tw);
    tw->setTabsClosable(true);

    if (withDefaultTab) {
        newTab(tw);
    } else {
        // Still add "+" tab
        QWidget* plusTab = new QWidget();
        tw->addTab(plusTab, "+");
        tw->tabBar()->setTabButton(tw->count() - 1, QTabBar::RightSide, nullptr);
    }

    connect(tw, &QTabWidget::tabCloseRequested, this, [=](int index) {
        if (tw->tabText(index) == "+") {
            return;
        }

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
    QTabWidget* newTabView = createTabWidget(true);
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
    if (QMessageBox::question(this, "Reset Layout", "Are you sure you want to close all tabs? This will close all tabs and remove all splits, including code") 
    != QMessageBox::Yes) 
    {
        return; // User cancelled
    }

    closeAllTabsInternal();
    
    emit allTabsClosed(); // Notify that all tabs are closed
}

void TabScriptEditor::closeAllTabsInternal()
{


        // Remove all tabs from each tab widget, including "+" tabs
    for (QTabWidget* tw : tabWidgets) {
        while (tw->count() > 0) {
            QWidget* w = tw->widget(tw->count() - 1);
            tw->removeTab(tw->count() - 1);
            if (w) w->deleteLater();
        }
    }

    // Remove and delete all tab widgets from splitter
    while (splitter->count() > 0) {
        QWidget* w = splitter->widget(0);
        if (w) {
            w->setParent(nullptr); 
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


void TabScriptEditor::saveState()
{
    QSettings settings("Robin", "BesEditor");

    settings.beginGroup("TabScriptEditor");

    settings.setValue("geometry", saveGeometry()); 
    // Save splitter state (sizes)
    QList<int> sizes = splitter->sizes();
    settings.setValue("splitterSizes", QVariant::fromValue(sizes));

    // Save number of tab widgets (splits)
    settings.setValue("tabWidgetCount", tabWidgets.size());

    // Save tabs per tab widget
    settings.beginWriteArray("tabWidgets");
    for (int i = 0; i < tabWidgets.size(); ++i) {
        settings.setArrayIndex(i);
        QTabWidget* tw = tabWidgets[i];

        // Save tabs in this tab widget
        settings.beginWriteArray("tabs");
        int tabCount = 0;
        for (int j = 0; j < tw->count(); ++j) {
            if (tw->tabText(j) == "+") continue; // skip plus tab
            if (auto editor = qobject_cast<CodeEditor*>(tw->widget(j))) {
                settings.setArrayIndex(tabCount++);
                settings.setValue("content", editor->toPlainText());
                settings.setValue("tabText", tw->tabText(j));
            }
        }
        settings.endArray();
    }
    settings.endArray();

    // Save where the editor was docked
    if (auto dock = qobject_cast<QDockWidget*>(parentWidget())) {
        if (auto mw = qobject_cast<QMainWindow*>(dock->parentWidget())) {
            settings.setValue("mainWindowState", mw->saveState());
        }
    }



    settings.endGroup();

    qDebug() << "Saved" << tabWidgets.size() << "tab widgets.";
}


void TabScriptEditor::loadState()
{
    QSettings settings("Robin", "BesEditor");

    settings.beginGroup("TabScriptEditor");

    // Load geometry of the editor
    restoreGeometry(settings.value("geometry").toByteArray());

    // Clear current tabs and widgets
    closeAllTabsInternal(); 

    // Read splitter sizes
    QVariant v = settings.value("splitterSizes");
    QList<int> sizes = v.isValid() ? v.value<QList<int>>() : QList<int>();

    // Number of tab widgets (splits)
    int count = settings.value("tabWidgetCount", 1).toInt();

    // Read tabWidgets array
    int savedTabWidgetCount = settings.beginReadArray("tabWidgets");

    for (int i = 0; i < savedTabWidgetCount; ++i) {
        settings.setArrayIndex(i);

        QTabWidget* tw = createTabWidget(false);
        splitter->addWidget(tw);

        // Read tabs for this tabWidget
        int tabCount = settings.beginReadArray("tabs");
        for (int j = 0; j < tabCount; ++j) {
            settings.setArrayIndex(j);

            CodeEditor* editor = new CodeEditor();
            editor->setPlainText(settings.value("content", "").toString());
            editor->installEventFilter(this);
            new Highlighter(editor->document());

            QString tabText = settings.value("tabText", "py").toString();

            tw->addTab(editor, tabText);
        }
        settings.endArray();

        // Remove all "+" tabs if any, then add exactly one
        for (int k = tw->count() - 1; k >= 0; --k) {
            if (tw->tabText(k) == "+") {
                QWidget* w = tw->widget(k);
                tw->removeTab(k);
                w->deleteLater();
            }
        }
        QWidget* plusTab = new QWidget();
        tw->addTab(plusTab, "+");
        tw->tabBar()->setTabButton(tw->count() - 1, QTabBar::RightSide, nullptr);
    }
    settings.endArray();

    // Restore splitter sizes if valid
    if (sizes.size() == splitter->count()) {
        splitter->setSizes(sizes);
    }
    
    //restore docking
    if (auto dock = qobject_cast<QDockWidget*>(parentWidget())) {
        if (auto mw = qobject_cast<QMainWindow*>(dock->parentWidget())) {
            mw->restoreState(settings.value("mainWindowState").toByteArray());
        }
    }

    settings.endGroup();

    qDebug() << "Loaded" << savedTabWidgetCount << "tab widgets with tabs.";
}




void TabScriptEditor::resetLayout()
{
    // open a confirmation dialog
    if (QMessageBox::question(this, "Reset Layout", "Are you sure you want to reset the layout? This will close all tabs and remove all splits, including code") 
    != QMessageBox::Yes) 
    {
        return; 
    }



    // Clear current layout and delete child widgets
    QLayout* currentLayout = this->layout();
    if (currentLayout) {
        QLayoutItem* item;
        while ((item = currentLayout->takeAt(0)) != nullptr) {
            if (QWidget* w = item->widget()) {
                w->setParent(nullptr);
                w->deleteLater();
            }
            delete item;
        }
        delete currentLayout;
    }

    // Clear existing splitter and tabWidgets container
    if (splitter) {
        splitter->setParent(nullptr);
        splitter->deleteLater();
        splitter = nullptr;
    }
    tabWidgets.clear();
    focusedTabWidget = nullptr;

    // Recreate layout and widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    splitter = new QSplitter(Qt::Horizontal, this);
    layout->addWidget(splitter);

    QTabWidget* tw = createTabWidget(true);
    splitter->addWidget(tw);

    this->setStyleSheet(Style::tabStyle);
    this->updateGeometry();
    this->adjustSize();
    this->update();
}

