#ifndef IMPORT_DUMMY_PLUGIN_H
#define IMPORT_DUMMY_PLUGIN_H

#include <QObject>
#include <QThread>
#include <translationeditorimportplugininterface.h>

#include "importdummyplugin_global.h"

class ImportDummyPluginWorker;

//==============================================================================
// Translation Editor Import Dummy Plugin Class
//==============================================================================
class IMPORT_DUMMY_PLUGIN_SHARED_EXPORT ImportDummyPlugin: public ITranslationEditorImportPluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.harman.translationeditor.import.plugin" FILE "ImportDummyPlugin.json")

    Q_INTERFACES(ITranslationEditorImportPluginInterface)

public:
    // Constructor
    explicit ImportDummyPlugin(QObject* aParent = nullptr);

    // Destructor
    ~ImportDummyPlugin();

public: // From ITranslationEditorImportPluginInterface
    // Launch Import
    virtual QJsonObject launchImport();

    // Get Open File/Dir Path
    virtual QString getOpenPath();

    // Import TO Package
    virtual QJsonObject importToPackage(const QString& aPath);

    // Import To Package Async
    virtual void importToPackageAsync(const QString& aPath, const QJsonObject& aPackageObject);

signals:
    // Trigger Import Worker Signal
    void triggerImport();

protected slots:
    // Worker Finished
    void workerFinished();

    // ...

protected:
    // Worker Thread
    QThread                     mWorkerThread;
    // Worker
    ImportDummyPluginWorker*    mWorker;
};










//==============================================================================
// Worker Class For Dummy Import
//==============================================================================
class ImportDummyPluginWorker : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit ImportDummyPluginWorker(QJsonObject* aObject, QObject* aParent = nullptr);

    // Destructor
    ~ImportDummyPluginWorker();

public slots:
    // Do Import
    void doImport();

signals:
    // Ready Signal
    void importReady();

protected:
    // JSON Object For Import
    QJsonObject*    mImportObject;
};


#endif // IMPORT_DUMMY_PLUGIN_H
