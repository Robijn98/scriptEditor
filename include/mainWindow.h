#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scriptEditorPanel.h"


#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>


QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //has no parent
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QString currentFile = "";
    ScriptEditorPanel *scriptEditorDock;

};

#endif // MAINWINDOW_H