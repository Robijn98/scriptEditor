#include <gtest/gtest.h>
#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QDebug>

//fetch environment variable with optional fallback
QString getEnvVar(const char* name, const QString& fallback = "")
{
    QByteArray value = qgetenv(name);
    if (value.isEmpty())
        return fallback;
    return QString::fromUtf8(value);
}

// Get Maya and plugin paths once for all tests
QString mayaPath = getEnvVar("MAYA_PATH", "/usr/autodesk/maya2023/bin/maya");
QString pluginPath = getEnvVar("PLUGIN_PATH", QDir::currentPath() + "/libbesEditor.so");

TEST(MayaTests, generalTest)
{
    EXPECT_TRUE(true);
}

TEST(MayaTests, pluginLoadTest) 
{
    QProcess process;
    process.start(mayaPath, {"-batch", "-command", QString("loadPlugin \"%1\"").arg(pluginPath)});
    process.waitForFinished(30000); 
    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "Plugin failed to load in headless Maya";
}

TEST(MayaTests, pluginUnloadTest)
{
    QProcess process;
    process.start(mayaPath, {"-batch", "-command", QString("unloadPlugin \"%1\"").arg(pluginPath)});
    process.waitForFinished(30000); 
    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "Plugin failed to unload in headless Maya";
}

TEST(MayaTests, scriptEditorCommandAvailableTest)
{
    QProcess process;
    process.start(mayaPath, {
        "-batch",
        "-command",
        QString("loadPlugin \"%1\"; if (`exists showBesEditor`) print(\"OK\"); else error(\"Command not found\");").arg(pluginPath)
    });
    process.waitForFinished(30000);

    QString output = process.readAllStandardOutput();
    QString errors = process.readAllStandardError();

    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "showBesEditor command missing\nSTDOUT:\n"
                           << output.toStdString()
                           << "\nSTDERR:\n" << errors.toStdString();
}

TEST(MayaTests, scriptEditorPanelTest)
{
    QProcess process;
    process.start(mayaPath, {
        "-batch",
        "-command",
        QString("loadPlugin \"%1\"; showBesEditor;").arg(pluginPath)
    });
    process.waitForFinished(30000);

    QString output = process.readAllStandardOutput();
    QString errors = process.readAllStandardError();

    int exitCode = process.exitCode();
    EXPECT_EQ(exitCode, 0) << "showBesEditor failed to open panel\nSTDOUT:\n"
                           << output.toStdString()
                           << "\nSTDERR:\n" << errors.toStdString();
}

