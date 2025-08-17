#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>
#include <QFileInfo>

class Config
{
public:
    // Paths to files and folders installed by drag-and-drop
    inline static QString syntaxPath;
    inline static QString riggingCommandsPath;
    inline static QString riggingTemplatesPath;
    inline static QString pluginFilePath;
    inline static QString mainScriptPath;

    // Initialize paths at runtime
    static void initialize()
    {
        // Maya user scripts directory
        QString userScriptsDir = QDir::homePath() + "/Documents/maya/scripts/";

        // Ensure general folder exists
        QDir generalDir(userScriptsDir);
        if (!generalDir.exists()) {
            generalDir.mkpath(".");
        }

        // Directories installed by drag-and-drop
        riggingCommandsPath = QDir(userScriptsDir + "riggingCommands").absolutePath();
        riggingTemplatesPath = QDir(userScriptsDir + "riggingTemplates").absolutePath();
        QString syntaxDir = QDir(userScriptsDir + "syntaxLists").absolutePath();

        // Syntax file path
        syntaxPath = QDir(syntaxDir).filePath("maya_cmds_list.txt");

        // Plugin file (assumes drag-and-drop installer copied it here)
        pluginFilePath = QDir(QDir::homePath() + "/Documents/maya/plug-ins").filePath("libbesEditor.so");
        
        // Main script path 
        mainScriptPath = QDir(userScriptsDir + "mainScript.py").absolutePath();
        
        // Ensure directories exist (creates them if missing)
        QDir().mkpath(riggingCommandsPath);
        QDir().mkpath(riggingTemplatesPath);
        QDir().mkpath(syntaxDir);
        QDir().mkpath(QFileInfo(pluginFilePath).absolutePath());
    }
};

#endif
