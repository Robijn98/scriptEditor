#include "commandList.h"

#include <QTextStream>
#include <QRegularExpression>
#include <QInputDialog>
#include <list>

#include "config.h"

CommandList::CommandList(QListWidget *parent)
: QListWidget(parent)

{
    std::list<QString> commandList;

    QDir dir(Config::riggingCommandsPath);

    commandList = convertToList(dir);
    populateList(commandList);

    //connnects the item being clicked to print the command definition and imports to the script editor
    connect(this, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        QString commandText = commandDef(item->text(), dir);
        emit commandSelected(commandText);
    });


    return;
}


QSize CommandList::sizeHint() const
{
    QSize hint;
    hint.setWidth(hint.width() * 0.5);
    return hint;
}


//read folder content into list
std::list<QString> CommandList::convertToList(QDir dir)
{
    std::list<QString> listOut;

    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        QString name = fileInfo.fileName();

        if(name.endsWith(".py", Qt::CaseInsensitive))
        {
            name.chop(3);
        }

        listOut.push_back(name);
    }

    return listOut;
}

//turn list into listitems and populate the command list
void CommandList::populateList(std::list<QString> commandList)
{

    for(const QString &item : commandList)
    {
        this->addItem(item);
    }
}

QString CommandList::commandDef(QString fileName, QDir dir)
{
    
    QString commandDef;
    QString completeFileName = QString("%1/%2.py").arg(dir.path()).arg(fileName);
    QFile file(completeFileName);

    QRegularExpression re(R"(def\s+([a-zA-Z_][a-zA-Z0-9_]*\(.*\))\s*:)");

    // check if the file exists and can be opened
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString importFile = QString("import %1").arg(fileName);
        commandDef.append(importFile);
        commandDef.append("\n\n");

        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }

        bool classAndFunctionFound = false;

        // Check for #CLASS AND FUNCTION block
        for (int i = 0; i < lines.size() - 1; ++i) {
            if (lines[i].trimmed() == "#CLASS AND FUNCTION") {
                QString nextLine = lines[i + 1].trimmed();
                if (nextLine.startsWith("#") && nextLine.contains("(") && nextLine.contains(")")) {
                    // Remove the leading '#'
                    QString functionCall = nextLine.mid(1).trimmed(); 
                    commandDef.append(functionCall + "\n\n");
                    classAndFunctionFound = true;
                    // stop after first match
                    break; 
                }
            }
        }

        //Find any info lines
        for (const QString &line : lines) {
            if (line.contains("#INFO")) {
                commandDef.append("\n");
                commandDef.append(line + "\n");
            }

            //Look for function definitions if no class was found
            if (!classAndFunctionFound) {
                QRegularExpressionMatch match = re.match(line);
                if (match.hasMatch()) {
                    QString funcSignature = match.captured(1);
                    commandDef.append(fileName + "." + funcSignature + "\n");
                }
            }
        }
    }

    return commandDef;
}



void CommandList::clearItemsManually()
{
    while(this->count() > 0) {
        QListWidgetItem *item = this->takeItem(0);
        delete item;
    }
}


void CommandList::refreshList()
{
    clearItemsManually();

    QDir dir(Config::riggingCommandsPath);
    std::list<QString> commandList;

    commandList = convertToList(dir);
    populateList(commandList);
}

void CommandList::rename()
{
    bool ok{};
    QString newName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);

    QListWidgetItem *item = this->currentItem();
        QString oldName = item->text();

        QDir dir(Config::riggingCommandsPath);
        QFile::rename(dir.filePath(oldName + ".py"), dir.filePath(newName + ".py"));
}



void CommandList::remove()
{
    QListWidgetItem *item = this->currentItem();
    QString fileName = item->text();


    QDir dir(Config::riggingCommandsPath);
    QFile::remove(dir.filePath(fileName + ".py"));
}
