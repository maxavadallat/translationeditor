#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

#include "importdummyplugin.h"
#include <translation_keys.h>

//==============================================================================
// Constructor
//==============================================================================
ImportDummyPlugin::ImportDummyPlugin(QObject* aParent)
    : ITranslationEditorImportPluginInterface(aParent)
    , mWorker(nullptr)
{
    qDebug() << "ImportDummyPlugin created.";

    // ...
}

//==============================================================================
// Launch Import
//==============================================================================
QJsonObject ImportDummyPlugin::launchImport()
{
    return importToPackage("");
}

//==============================================================================
// Get Open File/Dir Path
//==============================================================================
QString ImportDummyPlugin::getOpenPath()
{
    return "Dummy";
}

//==============================================================================
// Import Package
//==============================================================================
QJsonObject ImportDummyPlugin::importToPackage(const QString& aPath)
{
    qDebug() << "ImportDummyPlugin::importToPackage - aPath: " << aPath;

    // Init New Package Object
    QJsonObject newPackageObject;

    // Set Up Properties
    newPackageObject[PACKAGE_KEY_PACKAGE_NAME]      = QString("Dummy Package");
    newPackageObject[PACKAGE_KEY_FONTS_DIR]         = "";
    newPackageObject[PACKAGE_KEY_DEFAULT_FONT_NAME] = "";
    newPackageObject[PACKAGE_KEY_DEFAULT_FONT_SIZE] = 16;
    newPackageObject[PACKAGE_KEY_LANGUAGES]         = QString("de-DE;en-GB;en-US;hu-HU");

    // Init Group Array
    QJsonArray newPackageGroupArray;

    // Set Up Properties

    // Init Dummy Group
    QJsonObject newPackageGroup;

    // Set Up Properties
    newPackageGroup[PACKAGE_KEY_GROUP_NAME]         = QString("Dummy");

    // Init Translation Items Array
    QJsonArray newTranslationItems;

    // Generate Dummy Translations Info
    for (int i=0; i<10000; i++) {
        // Init New Translation Item JSON Object
        QJsonObject newTranslationItem;

        // Set Up Properties
        newTranslationItem[PACKAGE_KEY_TEXT_ID]         = QString("text_id_testing%1").arg(i, 5, 10, QChar('0'));
        newTranslationItem[PACKAGE_KEY_TEXT_GROUP]      = QString("Dummy");
        newTranslationItem[PACKAGE_KEY_TEXT_TYPE]       = QString("Dummy");
        newTranslationItem[PACKAGE_KEY_TEXT_STATE]      = QString("draft");
        newTranslationItem[PACKAGE_KEY_TEXT_ORIGINAL]   = QString("Testing %1").arg(i, 5, 10, QChar('0'));
        newTranslationItem[PACKAGE_KEY_TEXT_COMMENT]    = QString("Comment %1").arg(i, 5, 10, QChar('0'));

        // Skipping Layouts...

        // Init New Translations JSON Array
        QJsonArray newTranslationsArray;

        // Init New Translation Array Item JSON Object
        QJsonObject newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("de-DE");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Testen %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("en-GB");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Testing %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("en-US");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Testin %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("hu-HU");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Teszt %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationItem[PACKAGE_KEY_TEXT_TRANSLATIONS]   = newTranslationsArray;

        // Append New Translation Item
        newTranslationItems << newTranslationItem;
    }

    // Set Up Properties
    newPackageGroup[PACKAGE_KEY_TRANSLATION_ITEMS]  = newTranslationItems;

    // Append Dummy Group
    newPackageGroupArray << newPackageGroup;

    // Set Up Properties - Group Array
    newPackageObject[PACKAGE_KEY_GROUPS]            = newPackageGroupArray;

    return newPackageObject;
}

//==============================================================================
// Import Package Async
//==============================================================================
void ImportDummyPlugin::importToPackageAsync(const QString& aPath, const QJsonObject& aPackageObject)
{
    Q_UNUSED(aPackageObject);

    // Check Worker Thread
    if (mWorkerThread.isRunning()) {
        qDebug() << "ImportDummyPlugin::importToPackageAsync - Worker Still Running, Stopping First...";

        // Quit Worker Thread
        mWorkerThread.quit();
        // Wait For Thread To Finish
        mWorkerThread.wait();
    }

    qDebug() << "ImportDummyPlugin::importToPackageAsync - aPath: " << aPath;

    // Clear Import Package Content
    mImportPackage = QJsonObject();

    // Create New Worker
    mWorker = new ImportDummyPluginWorker(&mImportPackage);
    // Move To Thread
    mWorker->moveToThread(&mWorkerThread);

    // Connect Signals
    connect(&mWorkerThread, &QThread::finished, mWorker, &QObject::deleteLater);
    connect(this, &ImportDummyPlugin::triggerImport, mWorker, &ImportDummyPluginWorker::doImport);
    connect(mWorker, &ImportDummyPluginWorker::importReady, this, &ImportDummyPlugin::workerFinished);

    // Start Worker Thread
    mWorkerThread.start();

    // Emit Trigger Import Signal
    emit triggerImport();

    // ...

}

//==============================================================================
// Worker Finished
//==============================================================================
void ImportDummyPlugin::workerFinished()
{
    qDebug() << "ImportDummyPlugin::workerFinished";

    // Reset Worker Pointer
    mWorker = nullptr;

    // Emit Import Ready Signal
    emit importReady(mImportPackage);

    // Check Worker Thread
    if (mWorkerThread.isRunning()) {
        // Quit Thread
        mWorkerThread.quit();
        // Wait for Quit
        mWorkerThread.wait();
    }

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
ImportDummyPlugin::~ImportDummyPlugin()
{
    // Check Worker Thread
    if (mWorkerThread.isRunning()) {
        // Quit Worker Thread
        mWorkerThread.terminate();
    }

    // Check Worker
    if (mWorker) {
        mWorker->deleteLater();
        mWorker = nullptr;
    }

    // ...

    qDebug() << "ImportDummyPlugin deleted.";
}









//==============================================================================
// Constructor
//==============================================================================
ImportDummyPluginWorker::ImportDummyPluginWorker(QJsonObject* aObject, QObject* aParent)
    : QObject(aParent)
    , mImportObject(aObject)
{
    qDebug() << "ImportDummyPluginWorker created.";

    // ...
}

//==============================================================================
// Do Import
//==============================================================================
void ImportDummyPluginWorker::doImport()
{
    // Check Import Object
    if (!mImportObject) {
        // Emit Import Ready Signal
        emit importReady();

        return;
    }

    qDebug() << "ImportDummyPluginWorker::doImport - started.";

    // Set Up Properties
    (*mImportObject)[PACKAGE_KEY_PACKAGE_NAME]      = QString("Dummy Package");
    (*mImportObject)[PACKAGE_KEY_FONTS_DIR]         = "";
    (*mImportObject)[PACKAGE_KEY_DEFAULT_FONT_NAME] = "";
    (*mImportObject)[PACKAGE_KEY_DEFAULT_FONT_SIZE] = 16;
    (*mImportObject)[PACKAGE_KEY_LANGUAGES]         = QString("de-DE;en-GB;en-US;hu-HU");

    // Init Group Array
    QJsonArray newPackageGroupArray;

    // Set Up Properties

    // Init Dummy Group
    QJsonObject newPackageGroup;

    // Set Up Properties
    newPackageGroup[PACKAGE_KEY_GROUP_NAME]         = QString("Dummy");

    // Init Translation Items Array
    QJsonArray newTranslationItems;

    // Generate Dummy Translations Info
    for (int i=0; i<10000; i++) {
        // Init New Translation Item JSON Object
        QJsonObject newTranslationItem;

        // Set Up Properties
        newTranslationItem[PACKAGE_KEY_TEXT_ID]         = QString("text_id_testing%1").arg(i, 5, 10, QChar('0'));
        newTranslationItem[PACKAGE_KEY_TEXT_GROUP]      = QString("Dummy");
        newTranslationItem[PACKAGE_KEY_TEXT_TYPE]       = QString("Dummy");
        newTranslationItem[PACKAGE_KEY_TEXT_STATE]      = QString("draft");
        newTranslationItem[PACKAGE_KEY_TEXT_ORIGINAL]   = QString("Testing %1").arg(i, 5, 10, QChar('0'));
        newTranslationItem[PACKAGE_KEY_TEXT_COMMENT]    = QString("Comment %1").arg(i, 5, 10, QChar('0'));
        newTranslationItem[PACKAGE_KEY_TEXT_MAX_LENGTH] = -1;

        // Skipping Layouts...

        // Init New Translations JSON Array
        QJsonArray newTranslationsArray;

        // Init New Translation Array Item JSON Object
        QJsonObject newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("de-DE");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Testen %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("en-GB");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Testing %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("en-US");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Testin %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationTextItem[PACKAGE_KEY_TEXT_LANGUAGE]  = QString("hu-HU");
        newTranslationTextItem[PACKAGE_KEY_TEXT_TRANSLATED_TEXT]  = QString("Teszt %1").arg(i, 5, 10, QChar('0'));

        // Append Translation
        newTranslationsArray << newTranslationTextItem;

        // Set Up Properties
        newTranslationItem[PACKAGE_KEY_TEXT_TRANSLATIONS]   = newTranslationsArray;

        // Append New Translation Item
        newTranslationItems << newTranslationItem;
    }

    // Set Up Properties
    newPackageGroup[PACKAGE_KEY_TRANSLATION_ITEMS]  = newTranslationItems;

    // Append Dummy Group
    newPackageGroupArray << newPackageGroup;

    // Set Up Properties - Group Array
    (*mImportObject)[PACKAGE_KEY_GROUPS]            = newPackageGroupArray;

    qDebug() << "ImportDummyPluginWorker::doImport - done.";

    // Emit Import Ready Signal
    emit importReady();
}


//==============================================================================
// Destructor
//==============================================================================
ImportDummyPluginWorker::~ImportDummyPluginWorker()
{
    // ...

    qDebug() << "ImportDummyPluginWorker deleted.";
}
