#ifndef PLUGIN_INFO_H
#define PLUGIN_INFO_H

#include <QObject>
#include <QString>

//==============================================================================
// Plugin Info Class
//==============================================================================
class PluginInfo {
public:
    // Static Constructor - Load Plugin
    static PluginInfo loadPlugin(const QString& aPath, const bool& aUnloadInstance = true);

    // Constructor
    PluginInfo();

    // Name
    QString     mName;
    // Path
    QString     mPath;
    // Version
    QString     mVersion;
    // Menu Text
    QString     mMenuText;
    // Type - 0: Import, 1: Export
    int         mType;
    // Instance
    QObject*    mInstance;
};

#endif // PLUGIN_INFO_H
