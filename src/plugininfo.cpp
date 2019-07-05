#include <QDebug>
#include <QJsonObject>
#include <QPluginLoader>

#include "plugininfo.h"
#include "constants.h"
#include "shared/translationeditorexportplugininterface.h"
#include "shared/translationeditorimportplugininterface.h"

#define DEFAULT_KEY_METADATA            "MetaData"

#define DEFAULT_METADATA_KEY_NAME       "name"
#define DEFAULT_METADATA_KEY_TYPE       "type"
#define DEFAULT_METADATA_KEY_VERSION    "version"
#define DEFAULT_METADATA_KEY_MENU       "menu"

#define DEFAULT_PLUIN_TYPE_IMPORT       "import"
#define DEFAULT_PLUIN_TYPE_EXPORT       "export"

//==============================================================================
// Static Constructor - Load Plugin
//==============================================================================
PluginInfo PluginInfo::loadPlugin(const QString& aPath, const bool& aUnloadInstance)
{
    // Init New PLugin Info
    PluginInfo newPluginInfo;

    // Check Plugin File Path
    if (!aPath.isEmpty()) {
        // Init Plugin Loader
        QPluginLoader pluginLoader(aPath);

        // Get Plugin Metadata
        QJsonObject pluginMetaData = pluginLoader.metaData();
        // Get Plugin IID
        QString pluginIID = pluginMetaData.value("IID").toString();

        // Check IID
        if (!pluginIID.startsWith(DEFAULT_PLUGIN_IID_PREFIX)) {
            qWarning() << "PluginInfo::loadPlugin - aPath: " << aPath << " - PLUGIN IS NOT A TRANSLATION EDITOR PLUGIN!";
            return newPluginInfo;
        }

        // Load Plugin
        if (pluginLoader.load()) {
            // Set Path
            newPluginInfo.mPath = aPath;

            // Get Plugin Metadata Object
            QJsonObject metaDataObject = pluginMetaData.value(DEFAULT_KEY_METADATA).toObject();

            // Get Plugin Name
            newPluginInfo.mName = metaDataObject.value(DEFAULT_METADATA_KEY_NAME).toString();
            // Get Plugin Type
            QString pluginType = metaDataObject.value(DEFAULT_METADATA_KEY_TYPE).toString();

            // Check Pluin Type
            if (pluginType == DEFAULT_PLUIN_TYPE_IMPORT) {
                // Set PLugin Type
                newPluginInfo.mType = 0;
            }

            // Check Plugin Type
            if (pluginType == DEFAULT_PLUIN_TYPE_EXPORT) {
                // Set PLugin Type
                newPluginInfo.mType = 1;
            }

            // Get Plugin Version
            newPluginInfo.mVersion = metaDataObject.value(DEFAULT_METADATA_KEY_VERSION).toString();
            // Get Pluin Menu Item Text
            newPluginInfo.mMenuText = metaDataObject.value(DEFAULT_METADATA_KEY_MENU).toString();

            qDebug() << "PluginInfo::loadPlugin - name: " << newPluginInfo.mName;
            qDebug() << "PluginInfo::loadPlugin - type: " << newPluginInfo.mType;
            qDebug() << "PluginInfo::loadPlugin - version: " << newPluginInfo.mVersion;

            // ...

            // Check Unload Instance
            if (aUnloadInstance) {
                // Unload Plugin
                pluginLoader.unload();
            } else {
                // Set Instance
                newPluginInfo.mInstance = pluginLoader.instance();
            }
        }
    }

    return newPluginInfo;
}

//==============================================================================
// Constructor
//==============================================================================
PluginInfo::PluginInfo()
    : mName("")
    , mPath("")
    , mVersion("")
    , mMenuText("")
    , mType(-1)
    , mInstance(nullptr)
{

}
