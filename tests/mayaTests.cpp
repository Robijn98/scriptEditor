#include <gtest/gtest.h>
#include <QApplication>
#include <QProcess>


//check if test is running at all
TEST(MayaTests, generalTest)
 {
    EXPECT_TRUE(true);
}


TEST(MayaTests, pluginLoadTest) 
{
    QString mayaPath = "/usr/autodesk/maya2023/bin/maya";
    QString pluginPath = "/home/s5725067/myRepos/scriptEditor/build/libbesEditor.so";

    QProcess process;
    process.start(mayaPath, {"-batch", "-command", QString("loadPlugin \"%1\"").arg(pluginPath)});
    process.waitForFinished(30000); 
    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "Plugin failed to load in headless Maya";
}


TEST(MayaTests, pluginUnloadTest)
{
    QString mayaPath = "/usr/autodesk/maya2023/bin/maya";
    QString pluginPath = "/home/s5725067/myRepos/scriptEditor/build/libbesEditor.so";

    QProcess process;
    process.start(mayaPath, {"-batch", "-command", QString("unloadPlugin \"%1\"").arg(pluginPath)});
    process.waitForFinished(30000); 
    

    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "Plugin failed to unload in headless Maya";
}


TEST(MayaTests, scriptEditorOpenTest)
{
    QString mayaPath = "/usr/autodesk/maya2023/bin/maya";
    QString pluginPath = "/home/s5725067/myRepos/scriptEditor/build/libbesEditor.so";

    QProcess process;
    process.start(mayaPath, {"-batch", "-command", QString("showBesEditor")});
    process.waitForFinished(30000); 

    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "Script editor failed to open in headless Maya";
}

