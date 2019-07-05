#ifndef IMPORT_TS_PLUGIN_H
#define IMPORT_TS_PLUGIN_H

#include <QObject>
#include <QThread>
#include <translationeditorimportplugininterface.h>

#include "importtsplugin_global.h"

// Default TS File Suffix
#define DEFAULT_TS_FILE_SUFFIX              "ts"

#define XML_TOKEN_NAME_TS                   "TS"
#define XML_TOKEN_NAME_MESSAGE              "message"
#define XML_TOKEN_NAME_TRANSLATION          "translation"

#define XML_ATTRIBUTE_NAME_LANGUAGE         "language"
#define XML_ATTRIBUTE_NAME_VERSION          "version"
#define XML_ATTRIBUTE_NAME_ID               "id"


#define DEFAULT_NEW_PACKAGE_NAME            "New TS Package"

class ImportTSPluginWorker;

//==============================================================================
// Translation Editor Import TS Plugin Class
//==============================================================================
class IMPORT_TS_PLUGIN_SHARED_EXPORT ImportTSPlugin : public ITranslationEditorImportPluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.harman.translationeditor.import.plugin" FILE "ImportTSPlugin.json")

    Q_INTERFACES(ITranslationEditorImportPluginInterface)

public:
    // Constructor
    explicit ImportTSPlugin(QObject* aParent = nullptr);

    // Destructor
    ~ImportTSPlugin();

public: // From ITranslationEditorImportPluginInterface
    // Launch Import
    virtual QJsonObject launchImport();

    // Get Open File/Dir Path
    virtual QString getOpenPath();

    // Import To Package
    virtual QJsonObject importToPackage(const QString& aPath);

    // Import To a Package Async
    virtual void importToPackageAsync(const QString& aPath, const QJsonObject& aPackageObject);

signals:
    // Trigger Import Worker Signal
    void triggerImport();

protected slots:
    // Worker Finished
    void workerFinished();

    // ...

protected:
    // Last Open Dir Path
    QString                 mLastDirPath;
    // Worker Thread
    QThread                 mWorkerThread;
    // Worker
    ImportTSPluginWorker*   mWorker;
};




//==============================================================================
// Worker Class For TS Import
//==============================================================================
class ImportTSPluginWorker : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit ImportTSPluginWorker(const QString& aPath, QJsonObject* aObject, QObject* aParent = nullptr);

    // Destructor
    ~ImportTSPluginWorker();

public slots:
    // Do Import
    void doImport();

signals:
    // Ready Signal
    void importReady();

protected:
    // Import Path
    QString                         mImportPath;
    // JSON Object For Import
    QJsonObject*                    mImportObject;
    // Translation Map
    QMap<QString, TranslationInfo*> mTranslationMap;
};


#endif // IMPORT_TS_PLUGIN_H
