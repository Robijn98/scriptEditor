//Header file that encapsulates the OpenTemplate class, which is used to load or remove templates in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards

#ifndef OPENTEMPLATE_H
#define OPENTEMPLATE_H

#include <QDialog>
#include <QDir>

#include "scriptEditor.h"
#include "tabScriptEditor.h"

namespace Ui {
class OpenTemplate;
}

// Enum class to define the mode for opening templates
enum class TemplateMode {
    Load,
    Remove
};

class OpenTemplate : public QDialog
{
    Q_OBJECT

public:
    // Constructor for the OpenTemplate class   
    explicit OpenTemplate(TabScriptEditor* tabEditor, TemplateMode mode, QWidget *parent = nullptr);
    ~OpenTemplate();
    // Loads the template into the current editor
    void loadList();
    // removes the selected template
    void removeTemplate();

protected:
    // Converts the files in the directory to a list of template names
    std::list<QString> convertToList(QDir dir);

private slots:
    // Slot function that handles the loading of templates
    void on_loadButton_clicked();
    // closes the dialog without loading a template
    void on_cancelButton_clicked();

private:
    Ui::OpenTemplate *ui;
    CodeEditor *editor;
    // Populates the template list with items from a list of template names
    void populateList(std::list<QString> commandList);
    // Loads the template from a file into the current editor
    void loadTemplate(QString fileName, QDir dir);
    TemplateMode mode;
    TabScriptEditor* tabEditor;
    // Returns the current editor from the tab editor
    CodeEditor* currentEditor() const;
};

#endif // OPENTEMPLATE_H
