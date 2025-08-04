#include "tabScriptEditor.h"
#include <QVBoxLayout>
#include <QTabBar>
#include <QSplitter>
#include "style.h"

TabScriptEditor::TabScriptEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    splitter = new QSplitter(Qt::Horizontal, this);
    layout->addWidget(splitter);

    QTabWidget* tw = createTabWidget();
    splitter->addWidget(tw);
    focusedTabWidget = tw;

    this->setStyleSheet(Style::tabStyle);
}




void TabScriptEditor::newTab(QTabWidget* target)
{
    CodeEditor *editor = new CodeEditor();
    editor->installEventFilter(this); // Focus detection
    new Highlighter(editor->document());

    int index = target->count() - 1;
    target->insertTab(index, editor, tr("py"));
    target->setCurrentIndex(index);
}



void TabScriptEditor::newTab()
{
    QTabWidget* target = getFocusedTabWidget();
    if (target)
        newTab(target);
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

    QWidget* plusTab = new QWidget();
    tw->addTab(plusTab, "+");

    connect(tw, &QTabWidget::tabCloseRequested, this, [=](int index) {
        if (tw->tabText(index) != "+") {
            QWidget *w = tw->widget(index);
            tw->removeTab(index);
            delete w;
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
