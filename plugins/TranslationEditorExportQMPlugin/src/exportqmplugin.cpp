#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

#include "exportqmplugin.h"
#include "exporttsplugin.h"

#ifdef Q_OS_WIN
#define DEFAULT_LRELEASE_NAME   "lrelease.exe"
#else
#define DEFAULT_LRELEASE_NAME   "lrelease"
#endif

//==============================================================================
// Constructor
//==============================================================================
ExportQMPlugin::ExportQMPlugin(QObject* aParent)
    : ITranslationEditorExportPluginInterface(aParent)
    , mQtDir("")
    , mExportTS(nullptr)
    , mLastExportPath(QDir::homePath())
{
    qDebug() << "ExportQMPlugin created.";

    // Create Export TS Plugin
    mExportTS = new ExportTSPlugin(this);

    // ...
}

//==============================================================================
// Launch Export
//==============================================================================
void ExportQMPlugin::launchExport(const QJsonObject& aPackageObject)
{
    // Check Package Object
    if (!aPackageObject.isEmpty()) {
        //qDebug() << "ExportQMPlugin::launchExport";

        // Get Output Dir Path
        QString qmDirPath = QFileDialog::getExistingDirectory(nullptr, "Select Output Dir", mLastExportPath);

        // Check Output Dir Path
        if (!qmDirPath.isEmpty()) {
            qDebug() << "ExportQMPlugin::launchExport - qmlDirPath: " << qmDirPath;

            // Set Last Export Path
            mLastExportPath = qmDirPath;

            // Export Package
            exportPackage(aPackageObject, qmDirPath);
        }
    }
}

//==============================================================================
// Export Package
//==============================================================================
void ExportQMPlugin::exportPackage(const QJsonObject& aPackageObject, const QString& aPath)
{
    // Check Export TS Instance & Qt Dir
    if (mExportTS && !mQtDir.isEmpty()) {
        qDebug() << "ExportQMPlugin::exportPackage - aPath: " << aPath;

        // Export .ts Files
        mExportTS->exportPackage(aPackageObject, aPath);

        // Get lrelease path
        QString lreleasePath = mQtDir + "/bin/" + DEFAULT_LRELEASE_NAME;

        // Check If File Exists
        if (QFileInfo(lreleasePath).exists()) {

            // Init Command Line
            QString commandLine = QString("%1 -idbased %2/*.ts").arg(lreleasePath).arg(aPath);

            // Convert .ts Files to .qm
            int result = system(commandLine.toLocal8Bit());

            qDebug() << "ExportQMPlugin::exportPackage - result: " << result;

        } else {
            qWarning() << "ExportQMPlugin::exportPackage - CAN NOT FIND lrelease!";
        }

        // ...
    }
}

//==============================================================================
// Export Package Async
//==============================================================================
void ExportQMPlugin::exportPackageAsync(const QJsonObject& aPackageObject, const QString& aPath)
{
    // Check JSON Object
    if (!aPackageObject.isEmpty()) {
        qDebug() << "ExportQMPlugin::exportPackageAsync - aPath: " << aPath;

        // ...

    }
}

//==============================================================================
// Set Qt Dir
//==============================================================================
void ExportQMPlugin::setQtDir(const QString& aQtDir)
{
    qDebug() << "ExportQMPlugin::setQtDir - aQtDir: " << aQtDir;

    // Set Qt Dir
    mQtDir = aQtDir;
}

//==============================================================================
// Export TS Is Done Slot
//==============================================================================
void ExportQMPlugin::exportTSDone()
{
    qDebug() << "ExportQMPlugin::exportTSDone";

    // Make .qm files

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
ExportQMPlugin::~ExportQMPlugin()
{
    // ...

    qDebug() << "ExportQMPlugin deleted.";
}
