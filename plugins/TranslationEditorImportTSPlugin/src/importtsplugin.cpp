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

#include "importtsplugin.h"
#include <translation_keys.h>

//==============================================================================
// Constructor
//==============================================================================
ImportTSPlugin::ImportTSPlugin(QObject* aParent)
    : ITranslationEditorImportPluginInterface(aParent)
    , mWorker(nullptr)
{
    qDebug() << "ImportTSPlugin created.";

    // ...
}

//==============================================================================
// Launch Import
//==============================================================================
QJsonObject ImportTSPlugin::launchImport()
{
    return importToPackage(getOpenPath());
}

//==============================================================================
// Get Open File/Dir Path
//==============================================================================
QString ImportTSPlugin::getOpenPath()
{
    return QFileDialog::getExistingDirectory(nullptr, "Select Directory", QDir::homePath());;
}

//==============================================================================
// Import Package
//==============================================================================
QJsonObject ImportTSPlugin::importToPackage(const QString& aPath)
{
    qWarning() << "ImportTSPlugin::importToPackage - aPath: " << aPath << " - PLEASE USE ASYNC!!";

    // ...

    return QJsonObject();
}

//==============================================================================
// Import To a Package Async
//==============================================================================
void ImportTSPlugin::importToPackageAsync(const QString& aPath, const QJsonObject& aPackageObject)
{
    Q_UNUSED(aPackageObject);

    // Check Import Path
    if (aPath.isEmpty()) {
        qWarning() << "ImportTSPlugin::importToPackageAsync - EMPTY PATH!";

        // Empty Path

        // Emit Import Ready Signal
        emit importReady(QJsonObject());

        return;
    }

    // Check Worker Thread
    if (mWorkerThread.isRunning()) {
        qDebug() << "ImportTSPlugin::importToPackageAsync - Worker Still Running, Stopping First...";

        // Quit Worker Thread
        mWorkerThread.quit();
        // Wait For Thread To Finish
        mWorkerThread.wait();
    }

    qDebug() << "ImportTSPlugin::importToPackageAsync - aPath: " << aPath;

    // Clear Import Package Content
    mImportPackage = QJsonObject();

    // Create New Worker
    mWorker = new ImportTSPluginWorker(aPath, &mImportPackage);
    // Move To Thread
    mWorker->moveToThread(&mWorkerThread);

    // Connect Signals
    connect(&mWorkerThread, &QThread::finished, mWorker, &QObject::deleteLater);
    connect(this, &ImportTSPlugin::triggerImport, mWorker, &ImportTSPluginWorker::doImport);
    connect(mWorker, &ImportTSPluginWorker::importReady, this, &ImportTSPlugin::workerFinished);

    // Start Worker Thread
    mWorkerThread.start();

    // Emit Trigger Import Signal
    emit triggerImport();

    // ...
}

//==============================================================================
// Worker Finished
//==============================================================================
void ImportTSPlugin::workerFinished()
{
    qDebug() << "ImportTSlugin::workerFinished";

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
ImportTSPlugin::~ImportTSPlugin()
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

    qDebug() << "ImportTSPlugin deleted.";
}















//==============================================================================
// Constructor
//==============================================================================
ImportTSPluginWorker::ImportTSPluginWorker(const QString& aPath, QJsonObject* aObject, QObject* aParent)
    : QObject(aParent)
    , mImportPath(aPath)
    , mImportObject(aObject)
{
    qDebug() << "ImportTSPluginWorker created.";

    // ...
}

//==============================================================================
// Do Import
//==============================================================================
void ImportTSPluginWorker::doImport()
{
    // Check Import Object
    if (!mImportObject || mImportPath.isEmpty()) {
        // Emit Import Ready Signal
        emit importReady();

        return;
    }

    qDebug() << "ImportTSPluginWorker::doImport - mImportPath: " << mImportPath;

    // Random Seed
    qsrand(QDateTime::currentDateTime().time().msec());

    // Init TS Path File Info
    QFileInfo tsPathInfo(mImportPath);

    // Check If Dir
    if (tsPathInfo.isDir()) {
        qDebug() << "ImportTSPluginWorker::doImport - mImportPath: " << mImportPath << " - Directory";
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
            // Init Language Set
            QSet<QString> languageSet;

            // Init Package Object Values

            (*mImportObject)[PACKAGE_KEY_PACKAGE_NAME] = DEFAULT_NEW_PACKAGE_NAME;

            // ...

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
/*
                        qDebug() << "-------------------------------------------------------------------------";
                        //qDebug() << "ImportTSPluginWorker::doImport - tokeType: " << tsReader.tokenType();
                        qDebug() << "ImportTSPluginWorker::doImport - tokeString: " << tsReader.tokenString();
                        //qDebug() << "ImportTSPluginWorker::doImport - prefix: " << tsReader.prefix();
                        // Check Token Type
                        if (tsReader.tokenType() != QXmlStreamReader::Characters) {
                            qDebug() << "ImportTSPluginWorker::doImport - name: " << tsReader.name();
                        }
                        // Check Token Type
                        if (tsReader.tokenType() == QXmlStreamReader::Characters) {
                            qDebug() << "ImportTSPluginWorker::doImport - text: " << tsReader.text();
                        }
                        // Get Attributes Count
                        int attrCount = tsReader.attributes().count();
                        // Check Attributes Count
                        if (attrCount > 0) {
                            qDebug() << "ImportTSPluginWorker::doImport - attributes: " << attrCount;

                            // Iterate Through Attributes
                            for (int a=0; a<attrCount; a++) {
                                // Get Attribute
                                QXmlStreamAttribute attribute = tsReader.attributes()[a];
                                qDebug() << "ImportTSPluginWorker::doImport - attribute: " << attribute.name() << " - value: " << attribute.value();
                            }
                        }

                        qDebug() << "-------------------------------------------------------------------------";
*/
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

                                    //qDebug() << "\n";
                                    qDebug() << "=========================================================================";

                                    // Get Language
                                    currentLanguage = attributes.value(XML_ATTRIBUTE_NAME_LANGUAGE).toString();
                                    // Get Version
                                    QString version = attributes.value(XML_ATTRIBUTE_NAME_VERSION).toString();

                                    qDebug() << "ImportTSPluginWorker::doImport - currentLanguage: " << currentLanguage << " - version: " << version;

                                    // Add Current Language To Language Set
                                    languageSet << currentLanguage.replace('_', '-');

                                    // Reset Text Counter
                                    textCounter = 0;

                                    // ...

                                } else if (tsReader.name() == XML_TOKEN_NAME_MESSAGE) {

                                    //qDebug() << "-------------------------------------------------------------------------";

                                    // Get ID
                                    currentID = attributes.value(XML_ATTRIBUTE_NAME_ID).toString();

                                    //qDebug() << "ImportTSPluginWorker::doImport - currentID: " << currentID;

                                    // ...

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

                                    //qDebug() << "ImportTSPluginWorker::doImport - currentTranslation: " << currentTranslation;

                                    // ...
                                }
                            break;

                            case QXmlStreamReader::EndElement:
                                if (tsReader.name() == XML_TOKEN_NAME_TRANSLATION) {
                                    // Reset Translation Text Element Started
                                    trTextToken = false;

                                } else if (tsReader.name() == XML_TOKEN_NAME_MESSAGE) {

                                    //qDebug() << "-------------------------------------------------------------------------";

                                } else if (tsReader.name() == XML_TOKEN_NAME_TS) {

                                    qDebug() << "ImportTSPluginWorker::doImport - textCounter: " << textCounter;
                                    qDebug() << "=========================================================================";
                                    qDebug() << "\n";
                                }
                            break;

                            case QXmlStreamReader::Invalid:
                                qWarning() << "ImportTSPluginWorker::doImport - INVALID TOKEN!";
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

            // Init Language List
            QStringList languageList;
            // Iterate Through Language Set
            for (QString langSetItem : languageSet) {
                // Append Language To Language List
                languageList << langSetItem;
            }

            qDebug() << "ImportTSPluginWorker::doImport - languageList: " << languageList;

            // Set Available Languages
            (*mImportObject)[PACKAGE_KEY_LANGUAGES] = languageList.join(";");

            // Get Keys Count
            int kCount = mTranslationMap.keys().count();

            qDebug() << "ImportTSPluginWorker::doImport - mTranslationMap.keys: " << kCount;

            // Generate Random Index
            int randomIndex = qrand() % kCount;
            //qDebug() << "ImportTSPluginWorker::doImport - randomIndex: " << randomIndex;
            // Get Random Key
            QString randomKey = mTranslationMap.keys()[randomIndex];

            qDebug() << "ImportTSPluginWorker::doImport - randomKey: " << randomKey;
            qDebug() << "ImportTSPluginWorker::doImport - mTranslationMap.languages: " << mTranslationMap[randomKey]->mTextMap.keys();
            qDebug() << "ImportTSPluginWorker::doImport - mTranslationMap.translations: " << mTranslationMap[randomKey]->mTextMap.values();

            // Init New Groups
            QJsonArray newGroups;

            // Init New Goup Object
            QJsonObject newGroupObject;

            // Set Group Name
            newGroupObject[PACKAGE_KEY_GROUP_NAME] = DEFAULT_NEW_GROUP_NAME;

            // Init New Translation Items Array
            QJsonArray newTranslationItemsArray;

            // Iterate Through Translation Map
            for (QString textID : mTranslationMap.keys()) {
                // Get Translatin Info
                TranslationInfo* translatinInfo = mTranslationMap[textID];
                // Check Translation Info
                if (translatinInfo) {
                    // Init New Line Counter
                    int newLineCounter = 0;

                    // Init New Translation Item
                    QJsonObject newTranslationItem;

                    // Set New Translation Item Properties
                    newTranslationItem[PACKAGE_KEY_TEXT_ID] = textID;
                    newTranslationItem[PACKAGE_KEY_TEXT_GROUP] = DEFAULT_NEW_GROUP_NAME;
                    newTranslationItem[PACKAGE_KEY_TEXT_TYPE] = DEFAULT_NEW_TYPE;
                    newTranslationItem[PACKAGE_KEY_TEXT_STATE] = DEFAULT_NEW_STATE;
                    newTranslationItem[PACKAGE_KEY_TEXT_ORIGINAL] = translatinInfo->mTextMap[DEFAULT_NEW_ORIGINAL_TEXT_LANGUAGE];

                    // ...

                    // Init Translated Text Array
                    QJsonArray newTranslationItemTextArray;

                    // Iterate Through Translation Info Text Map
                    for (QString translationLanguage : translatinInfo->mTextMap.keys()) {
                        // Init New Translation Item Translated Text Object
                        QJsonObject newTranslationObject;

                        // Set New Translation Item Translated Text Object
                        newTranslationObject[PACKAGE_KEY_TEXT_LANGUAGE] = translationLanguage;

                        // Get Translated Text
                        QString translatedText = translatinInfo->mTextMap[translationLanguage];

                        // Update New Line Counter
                        newLineCounter = qMax(newLineCounter, translatedText.count('\n'));

                        newTranslationObject[PACKAGE_KEY_TEXT_TRANSLATED_TEXT] = translatedText.replace(QString("\\n"), QString("\n"));

                        // Append New Translation Item Translated Text Object
                        newTranslationItemTextArray << newTranslationObject;
                    }

                    // Add Translated Text's

                    newTranslationItem[PACKAGE_KEY_TEXT_TRANSLATIONS] = newTranslationItemTextArray;

                    // Check New Line Counter
                    if (newLineCounter > 0) {
                        // Init Layouts Array
                        QJsonArray newTranslationItemLayoutsArray;
                        // Init New Layout JSON Object
                        QJsonObject newTranslationItemLayout;
                        // Set Only Max Lines
                        newTranslationItemLayout[PACKAGE_KEY_TEXT_MAX_LINES] = newLineCounter + 1;
                        // Append New Layout Object To Layouts
                        newTranslationItemLayoutsArray << newTranslationItemLayout;
                        // Set Layouts
                        newTranslationItem[PACKAGE_KEY_TEXT_LAYOUTS] = newTranslationItemLayoutsArray;
                    }

                    // ...

                    // Append New Translation Item
                    newTranslationItemsArray << newTranslationItem;
                }
            }

            // Set Translations
            newGroupObject[PACKAGE_KEY_TRANSLATION_ITEMS] = newTranslationItemsArray;

            // Append New Group Object
            newGroups << newGroupObject;

            // Set Groups
            (*mImportObject)[PACKAGE_KEY_GROUPS] = newGroups;

            // ...
        }

    } else {
        qDebug() << "ImportTSPluginWorker::doImport - mImportPath: " << mImportPath << " - Single File";

        // ...
    }

    qDebug() << "ImportTSPluginWorker::doImport - done.";

    // Emit Import Ready Signal
    emit importReady();
}


//==============================================================================
// Destructor
//==============================================================================
ImportTSPluginWorker::~ImportTSPluginWorker()
{
    // ...

    qDebug() << "ImportTSPluginWorker deleted.";
}
