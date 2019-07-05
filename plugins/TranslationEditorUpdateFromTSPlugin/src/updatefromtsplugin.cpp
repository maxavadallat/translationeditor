#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>
#include <QSet>

#include <QXmlStreamReader>

#include "updatefromtsplugin.h"
#include <translation_keys.h>

//==============================================================================
// Constructor
//==============================================================================
UpdateFromTSPlugin::UpdateFromTSPlugin(QObject* aParent)
    : ITranslationEditorImportPluginInterface(aParent)
    , mWorker(nullptr)
{
    qDebug() << "UpdateFromTSPlugin created.";

    // ...
}

//==============================================================================
// Launch Import
//==============================================================================
QJsonObject UpdateFromTSPlugin::launchImport()
{
    return importToPackage(getOpenPath());
}

//==============================================================================
// Get Open File/Dir Path
//==============================================================================
QString UpdateFromTSPlugin::getOpenPath()
{
    return QFileDialog::getExistingDirectory(nullptr, "Select Directory", QDir::homePath());;
}

//==============================================================================
// Import Package
//==============================================================================
QJsonObject UpdateFromTSPlugin::importToPackage(const QString& aPath)
{
    qWarning() << "UpdateFromTSPlugin::importToPackage - aPath: " << aPath << " - PLEASE USE ASYNC!!";

    // ...

    return QJsonObject();
}

//==============================================================================
// Import To a Package Async
//==============================================================================
void UpdateFromTSPlugin::importToPackageAsync(const QString& aPath, const QJsonObject& aPackageObject)
{
    // Check Worker Thread
    if (mWorkerThread.isRunning()) {
        qDebug() << "UpdateFromTSPlugin::importToPackageAsync - Worker Still Running, Stopping First...";

        // Quit Worker Thread
        mWorkerThread.quit();
        // Wait For Thread To Finish
        mWorkerThread.wait();
    }

    qDebug() << "UpdateFromTSPlugin::importToPackageAsync - aPath: " << aPath;

    // Init Import Package Content
    mImportPackage = aPackageObject;

    // Create New Worker
    mWorker = new UpdateFromTSPluginWorker(aPath, &mImportPackage);
    // Move To Thread
    mWorker->moveToThread(&mWorkerThread);

    // Connect Signals
    connect(&mWorkerThread, &QThread::finished, mWorker, &QObject::deleteLater);
    connect(this, &UpdateFromTSPlugin::triggerImport, mWorker, &UpdateFromTSPluginWorker::doImport);
    connect(mWorker, &UpdateFromTSPluginWorker::importReady, this, &UpdateFromTSPlugin::workerFinished);

    // Start Worker Thread
    mWorkerThread.start();

    // Emit Trigger Import Signal
    emit triggerImport();

    // ...
}

//==============================================================================
// Worker Finished
//==============================================================================
void UpdateFromTSPlugin::workerFinished()
{
    qDebug() << "UpdateFromTSlugin::workerFinished";

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
UpdateFromTSPlugin::~UpdateFromTSPlugin()
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

    qDebug() << "UpdateFromTSPlugin deleted.";
}















//==============================================================================
// Constructor
//==============================================================================
UpdateFromTSPluginWorker::UpdateFromTSPluginWorker(const QString& aPath, QJsonObject* aObject, QObject* aParent)
    : QObject(aParent)
    , mImportPath(aPath)
    , mTargetObject(aObject)
{
    qDebug() << "UpdateFromTSPluginWorker created.";

    // ...
}

//==============================================================================
// Do Import
//==============================================================================
void UpdateFromTSPluginWorker::doImport()
{
    // Check Import Path and Target Object
    if (!mTargetObject || (*mTargetObject).isEmpty() || mImportPath.isEmpty()) {
        // Emit Import Ready Signal
        emit importReady();

        return;
    }

    qDebug() << "UpdateFromTSPluginWorker::doImport - mImportPath: " << mImportPath;

    // Init TS Path File Info
    QFileInfo tsPathInfo(mImportPath);

    // Language Set
    QSet<QString> languageSet;

    // Check If Dir
    if (tsPathInfo.isDir()) {
        qDebug() << "UpdateFromTSPluginWorker::doImport - mImportPath: " << mImportPath << " - Directory";
        // Init TS Dir
        QDir tsDir(mImportPath);

        // Init Name Filters
        QStringList nameFilters;
        // Append Filter
        nameFilters << QString("*.%1").arg(DEFAULT_TS_FILE_SUFFIX);

        // Get File Info List
        QFileInfoList tsList = tsDir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

        // Get TS Files Count
        int tsCount = tsList.count();

        // Check TS Files Count
        if (tsCount > 0) {
            //TS files use hash IDs
            bool useHashId = false;
            // Iterate Through TS Files
            for (int i=0; i<tsCount; i++) {

                // Init TS File
                QFile tsFile(tsList[i].absoluteFilePath());

                // Init Current Language
                QString currentLanguage = "";

                // Open TS File
                if (tsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    // Open TS File
                    QXmlStreamReader tsReader(&tsFile);

                    // Init Translation Text Token
                    bool trTextToken = false;
                    // Init Current Text ID
                    QString currentID = "";
                    // Init Current Translation
                    QString currentTranslation = "";

                    // Init Text Counter
                    int textCounter = 0;

                    // Iterate Through TS File
                    while (!tsReader.atEnd()) {
                        // Read Next
                        tsReader.readNext();

                        // Switch Toke Type
                        switch (tsReader.tokenType()) {
                            case QXmlStreamReader::StartDocument:
                            break;

                            case QXmlStreamReader::EndDocument:
                            break;

                            case QXmlStreamReader::StartElement: {
                                // Get Attributes
                                QXmlStreamAttributes attributes = tsReader.attributes();

                                // Check Name
                                if (tsReader.name() == XML_TOKEN_NAME_TS) {

                                    qDebug() << "=========================================================================";

                                    // Get Language
                                    currentLanguage = attributes.value(XML_ATTRIBUTE_NAME_LANGUAGE).toString();
                                    // Get Version
                                    QString version = attributes.value(XML_ATTRIBUTE_NAME_VERSION).toString();

                                    qDebug() << "UpdateFromTSPluginWorker::doImport - currentLanguage: " << currentLanguage << " - version: " << version;

                                    // Add Current Language To Language Set
                                    languageSet << currentLanguage.replace('_', '-');

                                    // Reset Text Counter
                                    textCounter = 0;

                                    // ...

                                } else if (tsReader.name() == XML_TOKEN_NAME_MESSAGE) {

                                    //qDebug() << "-------------------------------------------------------------------------";

                                    // Get ID
                                    if (attributes.hasAttribute(XML_ATTRIBUTE_NAME_ID)) {
                                        currentID = attributes.value(XML_ATTRIBUTE_NAME_ID).toString();
                                    } else {
                                        //if no ID is used, then get hash ID
                                        useHashId = true;
                                        currentID = attributes.value(XML_ATTRIBUTE_NAME_HASHID).toString();
                                    }

                                    // Inc Text Counter
                                    textCounter++;

                                } else if (tsReader.name() == XML_TOKEN_NAME_TRANSLATION) {

                                    // Set Translation Text Element Started
                                    trTextToken = true;
                                }
                            } break;

                            case QXmlStreamReader::Characters:
                                // Check Translation Text Element Started
                                if (trTextToken) {

                                    // Get Current Translation
                                    currentTranslation = tsReader.text().toString();

                                    if (DEFAULT_NEW_ORIGINAL_TEXT_LANGUAGE == currentLanguage) {
                                        mOriginalTextMap[currentTranslation] = currentID;
                                    }
                                    // Get Translation Info
                                    TranslationInfo* translationInfo = mTranslationMap.value(currentID);

                                    // Check Translation Info
                                    if (!translationInfo) {
                                        // Create New Translation Info
                                        translationInfo = new TranslationInfo();
                                    }

                                    // Set Translation Text Map
                                    translationInfo->mTextMap[currentLanguage] = currentTranslation;

                                    // Set Translation Map Value
                                    mTranslationMap[currentID] = translationInfo;
                                }
                            break;

                            case QXmlStreamReader::EndElement:
                                if (tsReader.name() == XML_TOKEN_NAME_TRANSLATION) {
                                    // Reset Translation Text Element Started
                                    trTextToken = false;

                                } else if (tsReader.name() == XML_TOKEN_NAME_MESSAGE) {

                                    //qDebug() << "-------------------------------------------------------------------------";

                                } else if (tsReader.name() == XML_TOKEN_NAME_TS) {

                                    qDebug() << "UpdateFromTSPluginWorker::doImport - textCounter: " << textCounter;
                                    qDebug() << "=========================================================================";
                                    qDebug() << "\n";
                                }
                            break;

                            case QXmlStreamReader::Invalid:
                                qWarning() << "UpdateFromTSPluginWorker::doImport - INVALID TOKEN!";
                            break;

                            default:
                            break;
                        }
                    }

                    // Close File
                    tsFile.close();
                }

                // ...

            }
        }

        //check if original language file exists
        if (languageSet.contains(DEFAULT_NEW_ORIGINAL_TEXT_LANGUAGE)) {


                qDebug() << "UpdateFromTSPluginWorker::doImport - updating JSON";

                // Init Language List
                QStringList languageList = (*mTargetObject).value(PACKAGE_KEY_LANGUAGES).toString().split(";");
                // Iterate Through Language Set
                for (QString langSetItem : languageSet) {
                    // Append Language To Language List
                    languageList << langSetItem;
                }
                //Remove duplicates
                languageList.removeDuplicates();
                // Sort Languages
                languageList.sort();

                qDebug() << "UpdateFromTSPluginWorker::doImport - languageList: " << languageList;

                // Set Available Languages
                (*mTargetObject)[PACKAGE_KEY_LANGUAGES] = languageList.join(";");


                // Get Groups Array
                QJsonArray groupsArray = (*mTargetObject).value(PACKAGE_KEY_GROUPS).toArray();

                // Get Group Count
                int gCount = groupsArray.count();

                qDebug() << "UpdateFromTSPluginWorker::doImport - gCount: " << gCount;

                // Iterate Through Groups
                for (int i=0; i<gCount; i++) {
                    // Get Group Object
                    QJsonObject group = groupsArray[i].toObject();

                    // Get Items
                    QJsonArray items = group.value(PACKAGE_KEY_TRANSLATION_ITEMS).toArray();

                    // Get Items Count
                    int iCount = items.count();

                    qDebug() << "UpdateFromTSPluginWorker::doImport - iCount: " << iCount;

                    // Load Items
                    for (int j=0; j<iCount; j++) {
                        // Gt Translation Item Object
                        QJsonObject trItem = items[j].toObject();
                        // Get Original Text
                        QString originalText = trItem.value(PACKAGE_KEY_TEXT_ORIGINAL).toString();

                        //search original text in TSTranslationsMap
                        if (mOriginalTextMap.contains(originalText))  {

                            QString textID = mOriginalTextMap[originalText];
                            qDebug() << "UpdateFromTSPluginWorker::doImport - textId: " << textID << " Original text: " << originalText;

                            TranslationInfo* translationInfo = mTranslationMap[textID];
                            // Init Translated Text Array

                            if (translationInfo) {
                                // Get Existing Translations
                                QJsonArray trItemTranslations = trItem.value(PACKAGE_KEY_TEXT_TRANSLATIONS).toArray();
                                // Iterate Through Translations
                                for (int tt=0; tt<trItemTranslations.count(); tt++) {
                                    // Get Translation
                                    QJsonObject trItemTranslation = trItemTranslations[tt].toObject();

                                    // Get Translation Language
                                    QString trItemTranslationLanguage = trItemTranslation.value(PACKAGE_KEY_TEXT_LANGUAGE).toString();

                                    // Check Translation Language
                                    if (!trItemTranslationLanguage.isEmpty()) {
                                        // Get Translated Text
                                        QString trItemTranslationText = trItemTranslation.value(PACKAGE_KEY_TEXT_TRANSLATED_TEXT).toString();

                                        if (translationInfo->mTextMap.contains(trItemTranslationLanguage) && !trItemTranslationText.isEmpty() ||
                                                !translationInfo->mTextMap.contains(trItemTranslationLanguage)) {
                                            translationInfo->mTextMap[trItemTranslationLanguage] = trItemTranslationText;
                                        }

                                    }
                                }
                                QJsonArray newTranslationItemTextArray;
                                for (QString translationLanguage : translationInfo->mTextMap.keys()) {
                                    // Init New Translation Item Translated Text Object
                                    QJsonObject newTranslationObject;

                                    // Set New Translation Item Translated Text Object
                                    newTranslationObject[PACKAGE_KEY_TEXT_LANGUAGE] = translationLanguage;

                                    // Get Translated Text
                                    QString translatedText = translationInfo->mTextMap[translationLanguage];

                                    newTranslationObject[PACKAGE_KEY_TEXT_TRANSLATED_TEXT] = translatedText.replace(QString("\\n"), QString("\n"));

                                    // Append New Translation Item Translated Text Object
                                    newTranslationItemTextArray << newTranslationObject;
                                }
                                trItem[PACKAGE_KEY_TEXT_TRANSLATIONS] = newTranslationItemTextArray;
                            }
                            items[j] = trItem;
                        }
                        else {
                            qDebug() << "UpdateFromTSPluginWorker::doImport - original text not found: " << originalText;
                        }
                     }
                    group[PACKAGE_KEY_TRANSLATION_ITEMS] = items;
                    groupsArray[i] = group;
                }
                (*mTargetObject)[PACKAGE_KEY_GROUPS] = groupsArray;
            }
            else {
                qDebug() << "UpdateFromTSPluginWorker::doImport - TS for original text not found " << DEFAULT_NEW_ORIGINAL_TEXT_LANGUAGE;
            }

    } else {
        qDebug() << "UpdateFromTSPluginWorker::doImport - mImportPath: " << mImportPath << " - Single File";

        // ...
    }

    qDebug() << "UpdateFromTSPluginWorker::doImport - done.";

    // Emit Import Ready Signal
    emit importReady();
}


//==============================================================================
// Destructor
//==============================================================================
UpdateFromTSPluginWorker::~UpdateFromTSPluginWorker()
{
    // ...

    qDebug() << "UpdateFromTSPluginWorker deleted.";
}
