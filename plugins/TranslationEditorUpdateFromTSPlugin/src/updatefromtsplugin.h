#ifndef IMPORT_TS_PLUGIN_H
#define IMPORT_TS_PLUGIN_H

#include <QObject>
#include <QThread>
#include <translationeditorimportplugininterface.h>

#include "updatefromtsplugin_global.h"

// Default TS File Suffix
#define DEFAULT_TS_FILE_SUFFIX              "ts"

#define XML_TOKEN_NAME_TS                   "TS"
#define XML_TOKEN_NAME_MESSAGE              "message"
#define XML_TOKEN_NAME_TRANSLATION          "translation"

#define XML_ATTRIBUTE_NAME_LANGUAGE         "language"
#define XML_ATTRIBUTE_NAME_VERSION          "version"
#define XML_ATTRIBUTE_NAME_ID               "id"
#define XML_ATTRIBUTE_NAME_HASHID           "hashId"


#define DEFAULT_NEW_PACKAGE_NAME            "New TS Package"

class UpdateFromTSPluginWorker;

//==============================================================================
// Translation Editor Import TS Plugin Class
//==============================================================================
class UPDATE_TS_PLUGIN_SHARED_EXPORT UpdateFromTSPlugin : public ITranslationEditorImportPluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.harman.translationeditor.import.plugin" FILE "UpdateFromTSPlugin.json")

    Q_INTERFACES(ITranslationEditorImportPluginInterface)

public:
    // Constructor
    explicit UpdateFromTSPlugin(QObject* aParent = nullptr);

    // Destructor
    ~UpdateFromTSPlugin();

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
    UpdateFromTSPluginWorker*   mWorker;
};




//==============================================================================
// Worker Class For TS Import
//==============================================================================
class UpdateFromTSPluginWorker : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit UpdateFromTSPluginWorker(const QString& aPath, QJsonObject* aObject, QObject* aParent = nullptr);

    // Destructor
    ~UpdateFromTSPluginWorker();

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
    QJsonObject*                    mTargetObject;
    // Translation Map
    QMap<QString, TranslationInfo*> mTranslationMap;    
    // Original text Map
    QMap<QString, QString> mOriginalTextMap;
};


#endif // IMPORT_TS_PLUGIN_H
