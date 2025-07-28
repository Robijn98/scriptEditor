#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <maya/MQtUtil.h>
#include <QtWidgets/QWidget>

inline QWidget* getMayaMainWindow()
{
    return MQtUtil::mainWindow();
}

#endif // MAINWINDOW_H
