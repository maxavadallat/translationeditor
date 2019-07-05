#ifndef TRANSLATIONEDITORIMPORTPLUGININTERFACE_H
#define TRANSLATIONEDITORIMPORTPLUGININTERFACE_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QMap>


#define DEFAULT_NEW_GROUP_NAME              "Core"

#define DEFAULT_NEW_ORIGINAL_TEXT_LANGUAGE  "en-US"

#define DEFAULT_NEW_TYPE                    "gui"

#define DEFAULT_NEW_STATE                   "draft"


//==============================================================================
// Translation Info Class
//==============================================================================
class TranslationInfo : public QObject
{
public:
    // Constructor
    explicit TranslationInfo(QObject* aParent = nullptr)
        : QObject(aParent) { }

    // Translated Text Map
    QMap<QString, QString> mTextMap;
};




//==============================================================================
// Translation Editor Import Plugin Interface Class
//==============================================================================
class ITranslationEditorImportPluginInterface : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit ITranslationEditorImportPluginInterface(QObject* aParent = nullptr) : QObject(aParent) { }

    // Destructor
    virtual ~ITranslationEditorImportPluginInterface() {
        // Iterate Through Translation Map
        while (mTranslationMap.keys().count() > 0) {
            // Get Last Key
            QString lastKey = mTranslationMap.keys().last();
            // Take Translation Info
            delete mTranslationMap.take(lastKey);
            // Remove Entry
            mTranslationMap.remove(lastKey);
        }
    }

public:
    // Launch Import
    virtual QJsonObject launchImport() = 0;

    // Get Open File/Dir Path
    virtual QString getOpenPath() = 0;

    // Import To Package
    virtual QJsonObject importToPackage(const QString& aPath) = 0;
    // Import To Package Async
    virtual void importToPackageAsync(const QString& aPath, const QJsonObject& aPackageObject) = 0;

    // ...

signals:
    // Import Ready Signal
    void importReady(const QJsonObject& aImportedPackage);

protected:
    // New Package to do the async import
    QJsonObject                         mImportPackage;
    // Translation Map
    QMap<QString, TranslationInfo*>     mTranslationMap;
};

Q_DECLARE_INTERFACE(ITranslationEditorImportPluginInterface, "org.harman.importplugininterface")


#endif // TRANSLATIONEDITORIMPORTPLUGININTERFACE_H
