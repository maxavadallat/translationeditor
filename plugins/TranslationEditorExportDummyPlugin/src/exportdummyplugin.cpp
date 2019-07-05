#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

#include "exportdummyplugin.h"
#include "translation_keys.h"

//==============================================================================
// Constructor
//==============================================================================
ExportDummyPlugin::ExportDummyPlugin(QObject* aParent)
    : ITranslationEditorExportPluginInterface(aParent)
{
    qDebug() << "ExportDummyPlugin created.";

    // ...
}

//==============================================================================
// Launch Export
//==============================================================================
void ExportDummyPlugin::launchExport(const QJsonObject& aPackageObject)
{
    qDebug() << "ExportDummyPlugin::launchExport";

    // Export Package - Dump
    exportPackage(aPackageObject, "");

    // ...
}

//==============================================================================
// Export Package
//==============================================================================
void ExportDummyPlugin::exportPackage(const QJsonObject& aPackageObject, const QString& aPath)
{
    // Check JSON Object
    if (!aPackageObject.isEmpty()) {
        qDebug() << "ExportDummyPlugin::exportPackage - aPath: " << aPath;

        qDebug() << "ExportDummyPlugin::exportPackage - package name     : " << aPackageObject.value(PACKAGE_KEY_PACKAGE_NAME).toString();
        qDebug() << "ExportDummyPlugin::exportPackage - fonts dir        : " << aPackageObject.value(PACKAGE_KEY_FONTS_DIR).toString();
        qDebug() << "ExportDummyPlugin::exportPackage - default font name: " << aPackageObject.value(PACKAGE_KEY_DEFAULT_FONT_NAME).toString();
        qDebug() << "ExportDummyPlugin::exportPackage - default font size: " << aPackageObject.value(PACKAGE_KEY_DEFAULT_FONT_SIZE).toInt();
        qDebug() << "ExportDummyPlugin::exportPackage - languages        : " << aPackageObject.value(PACKAGE_KEY_LANGUAGES).toString();

        // Get Groups
        QJsonArray groups = aPackageObject.value(PACKAGE_KEY_GROUPS).toArray();
        // Get Groups Count
        int gCount = groups.count();


        qDebug() << "ExportDummyPlugin::exportPackage - groups           : " << gCount;

        // Iterate Through Groups
        for (int g=0; g<gCount; g++) {
            // Get Group
            QJsonObject group = groups[g].toObject();

            qDebug() << "==================================================================";

            qDebug() << "ExportDummyPlugin::exportPackage - group name       : " << group.value(PACKAGE_KEY_GROUP_NAME).toString();

            // Get Translation Items
            QJsonArray translationItems = group.value(PACKAGE_KEY_TRANSLATION_ITEMS).toArray();

            // Get Translation Items Count
            int tCount = translationItems.count();

            qDebug() << "ExportDummyPlugin::exportPackage - translations     : " << tCount;

            // Iterate Through Translation Items
            for (int ti=0; ti<tCount; ti++) {
                qDebug() << "------------------------------------------------------------------";
                // Get Translation Item
                QJsonObject translationItem = translationItems[ti].toObject();

                qDebug() << "ExportDummyPlugin::exportPackage - text id          : " << translationItem.value(PACKAGE_KEY_TEXT_ID).toString();
                qDebug() << "ExportDummyPlugin::exportPackage - group            : " << translationItem.value(PACKAGE_KEY_TEXT_GROUP).toString();
                qDebug() << "ExportDummyPlugin::exportPackage - type             : " << translationItem.value(PACKAGE_KEY_TEXT_TYPE).toString();
                qDebug() << "ExportDummyPlugin::exportPackage - state            : " << translationItem.value(PACKAGE_KEY_TEXT_STATE).toString();
                qDebug() << "ExportDummyPlugin::exportPackage - original text    : " << translationItem.value(PACKAGE_KEY_TEXT_ORIGINAL).toString();
                qDebug() << "ExportDummyPlugin::exportPackage - comment          : " << translationItem.value(PACKAGE_KEY_TEXT_COMMENT).toString();

                // Get Layouts
                QJsonArray layouts = translationItem.value(PACKAGE_KEY_TEXT_LAYOUTS).toArray();
                // Get Layouts Count
                int lCount = layouts.count();

                qDebug() << "ExportDummyPlugin::exportPackage - layouts          : " << lCount;

                // Iterate Through Layouts
                for (int l=0; l<lCount; l++) {
                    // Get Layout
                    QJsonObject layout = layouts[l].toObject();
                    // Get Default
                    QString defaultText = layout.value(PACKAGE_KEY_TEXT_LAYOUT_DEFAULT).toString();
                    qDebug() << "ExportDummyPlugin::exportPackage - default layout   : " << defaultText;

                    // Get Font Name
                    QString fontName = layout.value(PACKAGE_KEY_TEXT_FONT_NAME).toString();
                    qDebug() << "ExportDummyPlugin::exportPackage - font name        : " << fontName;
                    // Get Font Size
                    int fontSize = layout.value(PACKAGE_KEY_TEXT_FONT_SIZE).toInt();
                    qDebug() << "ExportDummyPlugin::exportPackage - font size        : " << fontSize;
                    // Get Max Width
                    int maxWidth = layout.value(PACKAGE_KEY_TEXT_MAX_WIDTH).toInt();
                    qDebug() << "ExportDummyPlugin::exportPackage - max width        : " << maxWidth;
                    // Get Max Height
                    int maxHeight = layout.value(PACKAGE_KEY_TEXT_MAX_HEIGHT).toInt();
                    qDebug() << "ExportDummyPlugin::exportPackage - max height       : " << maxHeight;
                    // Get Max Lines
                    int maxLines = layout.value(PACKAGE_KEY_TEXT_MAX_LINES).toInt();
                    qDebug() << "ExportDummyPlugin::exportPackage - max lines        : " << maxLines;
                }

                // Get Translations
                QJsonArray translations = translationItem.value(PACKAGE_KEY_TEXT_TRANSLATIONS).toArray();
                // Get Translations Count
                int ttCount = translations.count();

                qDebug() << "ExportDummyPlugin::exportPackage - translations     : " << ttCount;

                // Iterate Through Translations
                for (int tt=0; tt<ttCount; tt++) {
                    // Get Translation Text
                    QJsonObject translation = translations[tt].toObject();

                    qDebug() << "ExportDummyPlugin::exportPackage - language         : " << translation.value(PACKAGE_KEY_TEXT_LANGUAGE).toString();
                    qDebug() << "ExportDummyPlugin::exportPackage - translated text  : " << translation.value(PACKAGE_KEY_TEXT_TRANSLATED_TEXT).toString();
                }

                qDebug() << "------------------------------------------------------------------";
            }

            qDebug() << "==================================================================";
        }

        // ...

    }
}

//==============================================================================
// Export Package Async
//==============================================================================
void ExportDummyPlugin::exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath)
{
    // Check JSON Object
    if (!aPackageObject.isEmpty()) {
        qDebug() << "ExportDummyPlugin::exportPackageAsync - aPath: " << aPath;

        // ...

    }
}

//==============================================================================
// Destructor
//==============================================================================
ExportDummyPlugin::~ExportDummyPlugin()
{
    // ...

    qDebug() << "ExportDummyPlugin deleted.";
}
