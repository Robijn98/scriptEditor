#include "config.h"
#include <QDebug>

void initializePluginPaths()
{
    // Must call this once at plugin startup
    Config::initialize();

    // Example usage
    qDebug() << "Syntax file path:" << Config::syntaxPath;
    qDebug() << "Rigging commands dir:" << Config::riggingCommandsPath;
    qDebug() << "Rigging templates dir:" << Config::riggingTemplatesPath;
    qDebug() << "Plugin path:" << Config::pluginFilePath;
}
