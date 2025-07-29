#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget* parent = nullptr);
    ~MyDialog() = default;
};

#endif // MYDIALOG_H
