#ifndef EXPORT_DUMMY_PLUGIN_H
#define EXPORT_DUMMY_PLUGIN_H

#include <QObject>
#include <translationeditorexportplugininterface.h>

#include "exportdummyplugin_global.h"

//==============================================================================
// Translation Edit Export Dummy Plugin Class
//==============================================================================
class EXPORT_DUMMY_PLUGIN_LIBRARY_EXPORT ExportDummyPlugin: public ITranslationEditorExportPluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.harman.translationeditor.export.plugin" FILE "ExportDummyPlugin.json")

    Q_INTERFACES(ITranslationEditorExportPluginInterface)

public:
    // Constructor
    explicit ExportDummyPlugin(QObject* aParent = nullptr);

    // Destructor
    ~ExportDummyPlugin();

public: // From ITranslationEditorExportPluginInterface
    // Launch Export
    virtual void launchExport(const QJsonObject& aPackageObject);

    // Export Package
    virtual void exportPackage(const QJsonObject& aPackageObject, const QString& aPath);

    // Export Package Async
    virtual void exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath);

    // ...

};

#endif // EXPORT_DUMMY_PLUGIN_H
