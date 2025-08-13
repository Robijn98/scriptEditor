//header file encapsulates the configuration class, which is used to manage the configuration settings in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef CONFIG_H
#define CONFIG_H

#pragma once
#include <QString>

class Config
{
public:


    // Static public variables
    inline static QString syntaxPath = "/home/s5725067/myRepos/scriptEditor/syntaxLists/maya_cmds_list.txt";
    inline static QString riggingCommandsPath = "/home/s5725067/myRepos/scriptEditorUI/riggingCommands";
    inline static QString riggingTemplatesPath = "/home/s5725067/myRepos/scriptEditorUI/riggingTemplates";
};

#endif
