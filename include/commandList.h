#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <QListWidget>
#include <QDir>

class CommandList : public QListWidget
{
    Q_OBJECT

public:
    explicit CommandList(QListWidget *parent = nullptr);
    QSize sizeHint() const override;
    QString commandDef(QString fileName, QDir dir);
    void refreshList();
    void rename();
    void remove();

protected:
    std::list<QString> convertToList(QDir dir);

signals:
    void commandSelected(const QString &commandText);

private:
    void populateList(std::list<QString> commandList);
    void clearItemsManually();

};

#endif
