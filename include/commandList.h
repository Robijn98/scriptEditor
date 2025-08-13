//header file encapsulates the command list class, which is used to manage the command list in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <QListWidget>
#include <QDir>

class CommandList : public QListWidget
{
    Q_OBJECT

public:
    // Constructor for the CommandList class
    explicit CommandList(QListWidget *parent = nullptr);
    // sets the size for the command list widget
    QSize sizeHint() const override;
    // prints the command and imports the command to the script editor
    QString commandDef(QString fileName, QDir dir);
    // refreshes the command list by clearing the items and populating it with the commands from the directory
    void refreshList();
    //renames the selected command in the command list
    void rename();
    // removes the selected command from the command list
    void remove();

protected:
    // reads python files from a directory and converts them into a list of command names
    std::list<QString> convertToList(QDir dir);

signals:
    // signal emitted when a command is selected from the command list
    void commandSelected(const QString &commandText);

private:
    // populates the command list with items from a list of command names
    void populateList(std::list<QString> commandList);
    // clears the items in the command list manually
    void clearItemsManually();

};

#endif
