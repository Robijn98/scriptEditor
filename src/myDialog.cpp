#include "myDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

MyDialog::MyDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("My Maya QDialog");

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Hello from QDialog!", this));

    auto* closeBtn = new QPushButton("Close", this);
    layout->addWidget(closeBtn);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}
