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
        // Maya sets MAYA_APP_DIR, which points to Documents/maya/<version>/
        // Example: C:/Users/John/Documents/maya/2025/
        QString mayaAppDir = qEnvironmentVariable("MAYA_APP_DIR", 
                                QDir::homePath() + "/Documents/maya/");

        // Scripts and plug-ins dirs under mayaAppDir
        QString userScriptsDir = QDir(mayaAppDir + "/scripts/").absolutePath();
        QString userPlugInsDir = QDir(mayaAppDir + "/plug-ins/").absolutePath();

        // Rigging folders
        riggingCommandsPath  = QDir(userScriptsDir + "/riggingCommands").absolutePath();
        riggingTemplatesPath = QDir(userScriptsDir + "/riggingTemplates").absolutePath();

        // Syntax list folder + file
        QString syntaxDir = QDir(userScriptsDir + "/syntaxLists").absolutePath();
        syntaxPath = QDir(syntaxDir).filePath("maya_cmds_list.txt");

        // Plugin file path
    #ifdef _WIN32
        pluginFilePath = QDir(userPlugInsDir).filePath("besEditor.mll");
    #else
        pluginFilePath = QDir(userPlugInsDir).filePath("libbesEditor.so");
    #endif

        // Main script path
        mainScriptPath = QDir(userScriptsDir).filePath("mainScript.py");

        // Ensure directories exist
        QDir().mkpath(riggingCommandsPath);
        QDir().mkpath(riggingTemplatesPath);
        QDir().mkpath(syntaxDir);
        QDir().mkpath(QFileInfo(pluginFilePath).absolutePath());
    }
};

#endif // CONFIG_H
