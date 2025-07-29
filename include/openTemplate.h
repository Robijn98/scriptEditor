#ifndef OPENTEMPLATE_H
#define OPENTEMPLATE_H

#include <QDialog>
#include <QDir>

#include "scriptEditor.h"

namespace Ui {
class OpenTemplate;
}

enum class TemplateMode {
    Load,
    Remove
};

class OpenTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit OpenTemplate(CodeEditor* editor, TemplateMode mode, QWidget *parent = nullptr);
    ~OpenTemplate();
    void loadList();
    void removeTemplate();

protected:
    std::list<QString> convertToList(QDir dir);

private slots:
    void on_loadButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::OpenTemplate *ui;
    CodeEditor *editor;
    void populateList(std::list<QString> commandList);
    void loadTemplate(QString fileName, QDir dir);
    TemplateMode mode;

};

#endif // OPENTEMPLATE_H
