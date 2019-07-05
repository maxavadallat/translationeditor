#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <QFile>
#include <QTextStream>

#include "exporttsplugin.h"
#include "translation_keys.h"

//==============================================================================
// Constructor
//==============================================================================
ExportTSPlugin::ExportTSPlugin(QObject* aParent)
    : ITranslationEditorExportPluginInterface(aParent)
{
    qDebug() << "ExportTSPlugin created.";

    // ...
}

//==============================================================================
// Launch Export
//==============================================================================
void ExportTSPlugin::launchExport(const QJsonObject& aPackageObject)
{
    // Get Output Dir Path
    QString tsDirPath = QFileDialog::getExistingDirectory(nullptr, "Select Output Dir", QDir::homePath());

    // Check Output Dir Path
    if (!tsDirPath.isEmpty()) {
        qDebug() << "ExportTSPlugin::launchExport - tsDirPath: " << tsDirPath;

        // Export Package
        exportPackage(aPackageObject, tsDirPath);
    }
}

//==============================================================================
// Export Package
//==============================================================================
void ExportTSPlugin::exportPackage(const QJsonObject& aPackageObject, const QString& aPath)
{
    // Init Output Dir Path File Info
    QFileInfo outputDirInfo(aPath);

    // Check Output Dir
    if (!outputDirInfo.exists() || !outputDirInfo.isDir()) {
        qWarning() << "ExportTSPlugin::exportPackage - INVALID OUTPUT DIR PATH!";

        return;
    }

    // Check JSON Object
    if (!aPackageObject.isEmpty()) {
        qDebug() << "ExportTSPlugin::exportPackage - aPath            : " << aPath;

        // Get Package Name
        QString packageName = aPackageObject.value(PACKAGE_KEY_PACKAGE_NAME).toString();

        qDebug() << "ExportTSPlugin::exportPackage - packageName      : " << packageName;

        // Init Languages
        QStringList languages = aPackageObject.value(PACKAGE_KEY_LANGUAGES).toString().split(";", QString::SkipEmptyParts);

        qDebug() << "ExportTSPlugin::exportPackage - languages        : " << languages;

        // Init File Map
        QMap<QString, QTextStream*> outFileMap;
        // Clear (!!!)
        outFileMap.clear();
        // Init New Text Stream
        QTextStream* newTSStream = nullptr;

        // Init Indent String
        QString indentString = "    ";

        // Iterate Through Languages
        for (QString language : languages) {
            //qDebug() << "ExportTSPlugin::exportPackage - language: " << language;

            // Init Language Replaced - to _
            QString languageReplaced = language;
            // Replace - with _
            languageReplaced.replace("-", "_");

            // Init TS File Name
            //QString tsFileName = packageName.replace(" ", "_") + "_" + languageReplaced + DEFAULT_TS_FILE_SUFFIX;
            QString tsFileName = language + DEFAULT_TS_FILE_SUFFIX;

            qDebug() << "ExportTSPlugin::exportPackage - tsFileName: " << tsFileName;

            // ...

            // Init Output TS File
            QFile* newTSFile = new QFile(aPath + "/" + tsFileName);

            // Open TS File For Write
            if (newTSFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
                // Create New Text Stream
                newTSStream = new QTextStream(newTSFile);

                // Add Stream To Output File Map
                outFileMap[language] = newTSStream;

                // Write Header
                (*newTSStream) << DEFAULT_XML_HEADER << "\n";
                // Write Root Element Begin
                (*newTSStream) << QString(DEFAULT_XML_ROOT_TS).arg(languageReplaced) << "\n";
                // Write Context Element Begin
                (*newTSStream) << indentString << DEFAULT_XML_ELEMENT_CONTEXT << "\n";
                // Write Name Element
                (*newTSStream) << indentString << indentString << DEFAULT_XML_ELEMENT_NAME << "\n";

                // ...

            } else {
                qWarning() << "ExportTSPlugin::exportPackage - CAN NOT OPEN FILE FOR WRITING: " << tsFileName;
                // Delete TS File
                delete newTSFile;
            }
        }

        // ...

        // Get Groups
        QJsonArray groups = aPackageObject.value(PACKAGE_KEY_GROUPS).toArray();

        qDebug() << "ExportTSPlugin::exportPackage - groups: " << groups.count();

        // Iterate Through Groups
        for (int g=0; g<groups.count(); g++) {
            // Get Group Object
            QJsonObject group = groups[g].toObject();

            qDebug() << "ExportTSPlugin::exportPackage - group: " << group.value(PACKAGE_KEY_GROUP_NAME).toString();

            // Get Translation Items
            QJsonArray trItems = group.value(PACKAGE_KEY_TRANSLATION_ITEMS).toArray();

            qDebug() << "ExportTSPlugin::exportPackage - trItems: " << trItems.count();

            // Iterate Through Translation Items
            for (int i=0; i<trItems.count(); i++) {
                // Get Translation Item Object
                QJsonObject trItem = trItems[i].toObject();

                // Get Text ID
                QString trTextID = trItem.value(PACKAGE_KEY_TEXT_ID).toString();

                //qDebug() << "ExportTSPlugin::exportPackage - trItem: " << trTextID;

                // Get Translations
                QJsonArray trItemTranslations = trItem.value(PACKAGE_KEY_TEXT_TRANSLATIONS).toArray();

                // Iterate Through Translations
                for (int tt=0; tt<trItemTranslations.count(); tt++) {
                    // Get Translation
                    QJsonObject trItemTranslation = trItemTranslations[tt].toObject();

                    // Get Translation Language
                    QString trItemTranslationLanguage = trItemTranslation.value(PACKAGE_KEY_TEXT_LANGUAGE).toString();

                    // Check Translation Language
                    if (!trItemTranslationLanguage.isEmpty()) {
                        // Get Translated Text
                        QString trItemTranslationText = trItemTranslation.value(PACKAGE_KEY_TEXT_TRANSLATED_TEXT).toString();

                        //qDebug() << "ExportTSPlugin::exportPackage - lang: " << trItemTranslationLanguage << " - trText: " << trItemTranslationText;

                        // Get Text Stream
                        QTextStream* tsStream = outFileMap.value(trItemTranslationLanguage);

                        // Check Stream
                        if (tsStream) {
                            // Write Message Element Begin
                            (*tsStream) << indentString << indentString << QString(DEFAULT_XML_ELEMENT_MESSAGE).arg(trTextID) << "\n";
//                            // Write Source Element
//                            (*tsStream) << indentString << indentString << indentString << DEFAULT_XML_ELEMENT_SOURCE << "\n";
                            // Write Translation Element
                            (*tsStream) << indentString << indentString << indentString << QString(DEFAULT_XML_ELEMENT_TRANSLATION).arg(trItemTranslationText) << "\n";
                            // Write Message Element End
                            (*tsStream) << indentString << indentString << DEFAULT_XML_ELEMENT_MESSAGE_END << "\n";
                        } else {
                            qCritical() << "ExportTSPlugin::exportPackage - STREAM NOT FOUND FOR LANGUAGE: " << trItemTranslationLanguage;
                        }
                    }
                }
            }
        }

        // ...

        qDebug() << "ExportTSPlugin::exportPackage - outFileMap.keys: " << outFileMap.keys();

        // Iterate Through Output File Map
        for (QString tsKey : outFileMap.keys()) {
            // Get Text Stream
            QTextStream* tsStream = outFileMap.value(tsKey);

            // Check Text Stream
            if (tsStream) {
                // Write Context Element End
                (*tsStream) << indentString << DEFAULT_XML_ELEMENT_CONTEXT_END << "\n";
                // Write Root Element End
                (*tsStream) << DEFAULT_XML_ROOT_TS_END << "\n";

                // Flush
                tsStream->flush();

                // Get File
                QFile* tsFile = (QFile*)(tsStream->device());

                // Close File
                tsFile->close();

                // Delete Stream
                delete tsStream;
                // Delete File
                delete tsFile;

            } else {
                qCritical() << "ExportTSPlugin::exportPackage - STREAM NOT FOUND FOR LANGUAGE: " << tsKey;
            }
        }

        // Clear Out File Map
        outFileMap.clear();
    }
}

//==============================================================================
// Export Package Async
//==============================================================================
void ExportTSPlugin::exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath)
{
    // Check JSON Object
    if (!aPackageObject.isEmpty()) {
        qDebug() << "ExportTSPlugin::exportPackageAsync - aPath: " << aPath;

        // ...

    }
}

//==============================================================================
// Destructor
//==============================================================================
ExportTSPlugin::~ExportTSPlugin()
{
    // ...

    qDebug() << "ExportTSPlugin deleted.";
}
