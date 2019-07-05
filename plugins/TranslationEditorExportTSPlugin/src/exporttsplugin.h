#ifndef EXPORT_TS_PLUGIN_H
#define EXPORT_TS_PLUGIN_H

#include <QObject>
#include <translationeditorexportplugininterface.h>

#include "exporttsplugin_global.h"

#define DEFAULT_TS_FILE_SUFFIX              ".ts"

#define DEFAULT_XML_HEADER                  "<?xml version='1.0' encoding='UTF-8'?>"

#define DEFAULT_XML_ROOT_TS                 "<TS language=\"%1\" version=\"2.1\">"
#define DEFAULT_XML_ROOT_TS_END             "</TS>"

#define DEFAULT_XML_ELEMENT_CONTEXT         "<context>"
#define DEFAULT_XML_ELEMENT_CONTEXT_END     "</context>"

#define DEFAULT_XML_ELEMENT_NAME            "<name />"

#define DEFAULT_XML_ELEMENT_MESSAGE         "<message id=\"%1\">"
#define DEFAULT_XML_ELEMENT_MESSAGE_END     "</message>"

#define DEFAULT_XML_ELEMENT_SOURCE          "<source />"

#define DEFAULT_XML_ELEMENT_TRANSLATION     "<translation>%1</translation>"


//==============================================================================
// Translation Editor Export TS Plugin Class
//==============================================================================
#ifndef EXPORT_QM_PLUGIN_USE
class EXPORT_TS_PLUGIN_LIBRARY_EXPORT ExportTSPlugin: public ITranslationEditorExportPluginInterface
#else
class ExportTSPlugin: public ITranslationEditorExportPluginInterface
#endif
{
    Q_OBJECT

#ifndef EXPORT_QM_PLUGIN_USE
    Q_PLUGIN_METADATA(IID "org.harman.translationeditor.export.plugin" FILE "ExportTSPlugin.json")

    Q_INTERFACES(ITranslationEditorExportPluginInterface)
#endif

public:
    // Constructor
    explicit ExportTSPlugin(QObject* aParent = nullptr);

    // Destructor
    ~ExportTSPlugin();

public: // From ITranslationEditorExportPluginInterface
    // Launch Export
    virtual void launchExport(const QJsonObject& aPackageObject);

    // Export Package
    virtual void exportPackage(const QJsonObject& aPackageObject, const QString& aPath);

    // Export Package Async
    virtual void exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath);

    // ...

};

#endif // EXPORT_TS_PLUGIN_H
