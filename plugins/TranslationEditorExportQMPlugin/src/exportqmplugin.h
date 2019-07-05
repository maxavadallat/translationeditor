#ifndef EXPORT_QM_PLUGIN_H
#define EXPORT_QM_PLUGIN_H

#include <QObject>
#include <translationeditorexportplugininterface.h>

#include "exportqmplugin_global.h"

class ExportTSPlugin;

//==============================================================================
// Translation Edit Export QM Plugin Class
//==============================================================================
class EXPORT_QM_PLUGIN_LIBRARY_EXPORT ExportQMPlugin: public ITranslationEditorExportPluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.harman.translationeditor.export.plugin" FILE "ExportQMPlugin.json")

    Q_INTERFACES(ITranslationEditorExportPluginInterface)

public:
    // Constructor
    explicit ExportQMPlugin(QObject* aParent = nullptr);

    // Destructor
    ~ExportQMPlugin();

public: // From ITranslationEditorExportPluginInterface
    // Launch Export
    virtual void launchExport(const QJsonObject& aPackageObject);

    // Export Package
    virtual void exportPackage(const QJsonObject& aPackageObject, const QString& aPath);

    // Export Package Async
    virtual void exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath);

    // Set Qt Dir
    virtual void setQtDir(const QString& aQtDir);

    // ...

protected slots:
    // Export TS Is Done Slot
    void exportTSDone();

protected: // Data
    // Qt Dir
    QString             mQtDir;

    // Export TS Plugin
    ExportTSPlugin*     mExportTS;

    // Last Export Path
    QString             mLastExportPath;
};

#endif // EXPORT_QM_PLUGIN_H
