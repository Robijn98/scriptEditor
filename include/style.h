#ifndef STYLE_H
#define STYLE_H

#pragma once
#include <QString>

class Style {
public:
    inline static QString buttonStyle =
        "background-color: black;"
        "border-style: solid;"
        "border-width: 2px;"
        "border-radius: 5px;"
        "border-color: #e36db4;"
        "font: bold 14px;"
        "min-width: 6em;"
        "padding: 6px;";

    inline static QString menuStyle =
        "color:#fff5fb;"
        "background-color:#e36db4;";

    inline static QString checkBoxStyle =
        "QCheckBox {"
        "    color: #fff5fb;"
        "    font: bold 14px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 16px;"
        "    height: 16px;"
        "    border: 2px solid #e36db4;"
        "    background-color: black;"
        "    border-radius: 3px;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: black;"
        "image: url(:/icons/icons/checkmark.png);"
        "}";

    inline static QString iconButtonStyle =
        "background-color: #1f1f1f;"
        "border-radius: 6px;"
        "border: 2px solid #e36db4;";

    inline static QString containerStyle =
        "color:#fff5fb;"
        "background-color:#1f1f1f;"
        "selection-color:#fff5fb;"
        "selection-background-color: #f5a9d6;";

    inline static QString listStyle =
        "QListWidget {"
        "color:#fff5fb;"
        "background-color:#1f1f1f;"
        "}"
        "QListWidget::item:selected {"
        " background: #eb46a9;"
        " border: 1px solid #eb46a9;"
        "}"
        "QListWidget::item:hover {"
        " background-color: #fcd2eb;"
        "}";

};


#endif // STYLE_H
