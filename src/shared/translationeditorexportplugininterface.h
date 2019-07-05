#ifndef TRANSLATIONEDITOREXPORTPLUGININTERFACE_H
#define TRANSLATIONEDITOREXPORTPLUGININTERFACE_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QThread>

//==============================================================================
// Translation Editor Export Plugin Interface Class
//==============================================================================
class ITranslationEditorExportPluginInterface : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit ITranslationEditorExportPluginInterface(QObject* aParent = nullptr) : QObject(aParent) { }
    // Destructor
    virtual ~ITranslationEditorExportPluginInterface() { }

public:
    // Launch Export
    virtual void launchExport(const QJsonObject& aPackageObject) = 0;

    // Export Package
    virtual void exportPackage(const QJsonObject& aPackageObject, const QString& aPath) = 0;
    // Export Package Async
    virtual void exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath) = 0;
    // Set Qt Dir
    virtual void setQtDir(const QString& aQtDir) { Q_UNUSED(aQtDir); }

signals:
    // Export Package Is Done
    void exportDone();

    // ...

};

Q_DECLARE_INTERFACE(ITranslationEditorExportPluginInterface, "org.harman.exportplugininterface")


#endif // TRANSLATIONEDITOREXPORTPLUGININTERFACE_H
