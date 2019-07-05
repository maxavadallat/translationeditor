#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QFileInfo>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "preferences.h"
#include "settings_defaults.h"

#include "preferencesdialog.h"
#include "aboutdialog.h"
#include "infodialog.h"
#include "confirmationdialog.h"
#include "packagepreferencesdialog.h"
#include "helpdialog.h"

#include "newgroupdialog.h"
#include "newtranslationdialog.h"
#include "alternatefontsdialog.h"
#include "finditemsdialog.h"

#include "packageinfo.h"
#include "packagegroup.h"
#include "packagegroupfilter.h"
#include "translationitem.h"
#include "translationitemlayout.h"
#include "shared/translation_keys.h"

#include "plugininfo.h"
#include "shared/translationeditorimportplugininterface.h"
#include "shared/translationeditorexportplugininterface.h"

#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
MainWindow::MainWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainWindow)
    , mBusy(false)
    , mPreferences(Preferences::getInstance())
    , mSupportedLanguages(QString(SUPPORTED_LANGUAGES).split(";"))
    , mRecentFiles()
    , mPreferencesDialog(nullptr)
    , mPackagePreferencesDialog(nullptr)
    , mAboutDialog(nullptr)
    , mInfoDialog(nullptr)
    , mConfirmDialog(nullptr)
    , mHelpDialog(nullptr)
    , mGroupDialog(nullptr)
    , mTranslationDialog(nullptr)
    , mAlternativeFontsDialog(nullptr)
    , mFindItemsDialog(nullptr)
    , mCurrentPackage(nullptr)
    , mCurrentPath("")
    , mCurrentGroup(nullptr)
    , mCurrentItem(nullptr)
    , mCurrentLayout(nullptr)
    , mCurrentIndex(-1)
    , mCurrentLayoutIndex(-1)
    , mPreviewWasShown(false)
    , mLastPreviewWidth(SETTINGS_DEFAULT_DEFAULT_PREVIEW_WIDTH)
    , mSelectedItemLanguage("")
    , mPreservedItemLanguage(-1)
    , mCellEditMode(false)
    , mCheckingActive(false)
    , mCheckingIndex(-1)
    , mLoadersCount(0)
{
    qDebug() << "MainWindow created.";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void MainWindow::init()
{
    qDebug() << "MainWindow::init";

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Register Components
    qmlRegisterUncreatableType<PackageInfo>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, CUSTOM_COMPONENT_NAME_PACKAGE_INFO, "");
    qmlRegisterUncreatableType<PackageGroup>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, CUSTOM_COMPONENT_NAME_PACKAGE_GROUP, "");
    qmlRegisterUncreatableType<TranslationItem>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, CUSTOM_COMPONENT_NAME_TRANSLATION_ITEM, "");
    qmlRegisterUncreatableType<TranslationItemLayout>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, CUSTOM_COMPONENT_NAME_TRANSLATION_LAYOUT, "");

    qmlRegisterType<PackageGroupFilter>(DEFAULT_CUSTOM_COMPONENTS, 1, 0, CUSTOM_COMPONENT_NAME_PACKAGE_FILTER);


    // Get Main Quick Widget Engine
    QQmlEngine* mainEngine = ui->mainQuickWidget->engine();
    // Get Main Quick Widget Context
    QQmlContext* mainContext = mainEngine ? mainEngine->rootContext() : nullptr;

    // Check Context
    if (mainContext) {
        // Set Context Property - Main Controller
        mainContext->setContextProperty(CONTEXT_PROPERTY_MAIN_CONTROLLER, this);
        // Set Context Property - Preferences
        mainContext->setContextProperty(CONTEXT_PROPERTY_PREFERENCES, mPreferences);
    }

    // Set Main Quick Widget Source
    ui->mainQuickWidget->setSource(QUrl(MAIN_QML_SOURCE));

    // Get Preview Quick Widget Engine
    QQmlEngine* previewEngine = ui->previewQuickWidget->engine();
    // Get Preview Quick Widget Context
    QQmlContext* previewContext = previewEngine ? previewEngine->rootContext() : nullptr;
    // Check Context
    if (previewContext) {
        // Set Context Property - Main Controller
        previewContext->setContextProperty(CONTEXT_PROPERTY_MAIN_CONTROLLER, this);
        // Set Context Property - Preferences
        previewContext->setContextProperty(CONTEXT_PROPERTY_PREFERENCES, mPreferences);
    }


    // Set Preview Quick Widget Source
    ui->previewQuickWidget->setSource(QUrl(PREVIEW_QML_SOURCE));

    // Build Language Map
    buildLanguageMap();

    // Load Recent Files List
    loadRecentFilesList();

    // Load Plugins
    loadPlugins();

    // Restore UI
    restoreUI();

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI()
{
    // Get Last Window State
    int lwState = mPreferences->lastWindowState();

    // Check Last Window State
    if (lwState == Qt::WindowNoState) {
        // Resize
        resize(mPreferences->lastWindowWidth(), mPreferences->lastWindowHeight());
    }

    // Init Main Splitter Sizes
    QList<int> sizes;

    // Set Last Preview Width
    mLastPreviewWidth = mPreferences->previewWidth();

    sizes << geometry().width();
    sizes << 0;

    // Set Sizes
    ui->mainSplitter->setSizes(sizes);
    // Set Font Size
    ui->fontSizeSpinner->setValue(DEFAULT_TEXT_FONT_SIZE);

    // Update Import/Export Submenu's
    updateImportExportSubMenu();
    // Update Recent File Submenu
    updateRecentFilesSubMenu();
    // Update Menu
    updateMenu();
    // Update Window Title
    updateWindowTitle();

    // ...
}

//==============================================================================
// Build Language Map
//==============================================================================
void MainWindow::buildLanguageMap()
{
    // Clear First
    mLanguageMap.clear();

    //qDebug() << "MainWindow::buildLanguageMap - mSupportedLanguages: " << mSupportedLanguages;

    // TODO: Have a better implementation for this

    mLanguageMap["ar-EG"] = "Arabic (Egypt)";
    mLanguageMap["bg-BG"] = "Bulgarian";
    mLanguageMap["bn-BN"] = "Bengali";
    mLanguageMap["cs-CZ"] = "Czech";
    mLanguageMap["da-DK"] = "Danish";
    mLanguageMap["de-AT"] = "German (AT)";
    mLanguageMap["de-CH"] = "German (CH)";
    mLanguageMap["de-DE"] = "German (DE)";
    mLanguageMap["de-LU"] = "German (LU)";
    mLanguageMap["el-GR"] = "Greek";
    mLanguageMap["en-AU"] = "English (AU)";
    mLanguageMap["en-CA"] = "English (CA)";
    mLanguageMap["en-GB"] = "English (GB)";
    mLanguageMap["en-IE"] = "English (IR)";
    mLanguageMap["en-NZ"] = "English (NZ)";
    mLanguageMap["en-US"] = "English (US)";
    mLanguageMap["es-AR"] = "Spanish (AR)";
    mLanguageMap["es-ES"] = "Spanish (ES)";
    mLanguageMap["es-MX"] = "Spanish (MX)";
    mLanguageMap["et-ET"] = "Estonian";
    mLanguageMap["fi-FI"] = "Finish";
    mLanguageMap["fr-FR"] = "French";
    mLanguageMap["he-IL"] = "Hebrew";
    mLanguageMap["hr-HR"] = "Croatian";
    mLanguageMap["hu-HU"] = "Hungarian";
    mLanguageMap["ic-IC"] = "Icelandic";
    mLanguageMap["it-IT"] = "Italian";
    mLanguageMap["ja-JA"] = "Japanese";
    mLanguageMap["ko-KO"] = "Korean";
    mLanguageMap["lt-LT"] = "Lithuanian";
    mLanguageMap["lv-LV"] = "Latvian";
    mLanguageMap["mt-MT"] = "Maltese";
    mLanguageMap["nb-NO"] = "Norwegian";
    mLanguageMap["nl-BE"] = "Dutch (BE)";
    mLanguageMap["nl-NL"] = "Dutch (NL)";
    mLanguageMap["pl-PL"] = "Polish";
    mLanguageMap["pt-BR"] = "Portugese (BR)";
    mLanguageMap["pt-PT"] = "Portugese (PT)";
    mLanguageMap["ro-RO"] = "Romanian";
    mLanguageMap["ru-RU"] = "Russian";
    mLanguageMap["sk-SK"] = "Slovak";
    mLanguageMap["sl-SL"] = "Slovenian";
    mLanguageMap["sr-SP"] = "Serbian";
    mLanguageMap["sv-SE"] = "Swedish";
    mLanguageMap["tr-TR"] = "Turkish";
    mLanguageMap["uk-UA"] = "Ukranian";
    mLanguageMap["zh-CN"] = "Chinese (CN)";
    mLanguageMap["zn-HK"] = "Chinese (HK)";

    // ...

    qDebug() << "MainWindow::buildLanguageMap - mLanguageMap: " << mLanguageMap;
}

//==============================================================================
// ShutDown
//==============================================================================
void MainWindow::shutDown()
{
    qDebug() << "MainWindow::shutDown";

    // Save Recent Files List
    saveRecentFilesList();

    // Close Package
    closePackage();

    // Clear Plugins
    clearPlugins();

    // Check Preferences
    if (mPreferences) {
        // Release Instance
        mPreferences->release();
    }

    // ...
}

//==============================================================================
// Get Current Package
//==============================================================================
PackageInfo* MainWindow::currentPackage()
{
    return mCurrentPackage;
}

//==============================================================================
// Get Current Group
//==============================================================================
PackageGroup* MainWindow::currentGroup()
{
    return mCurrentGroup;
}

//==============================================================================
// Get Current Translation Item
//==============================================================================
TranslationItem* MainWindow::currentItem()
{
    return mCurrentItem;
}

//==============================================================================
// Get Current Index
//==============================================================================
int MainWindow::currentIndex()
{
    return mCurrentIndex;
}

//==============================================================================
// Get Current Translation Item Layout
//==============================================================================
TranslationItemLayout* MainWindow::currentLayout()
{
    return mCurrentLayout;
}

//==============================================================================
// Get Recent Files
//==============================================================================
QStringList MainWindow::recentFiles()
{
    return mRecentFiles;
}

//==============================================================================
// Get Supported Languages
//==============================================================================
QStringList MainWindow::supportedLanguages()
{
    return mSupportedLanguages;
}

//==============================================================================
// Get Selected Translation Item Language
//==============================================================================
QString MainWindow::selectedItemLanguage()
{
    return mSelectedItemLanguage;
}

//==============================================================================
// Checking Mode Active
//==============================================================================
bool MainWindow::checkingActive()
{
    return mCheckingActive;
}

//==============================================================================
// Get Checking Index
//==============================================================================
int MainWindow::checkIndex()
{
    return mCheckingIndex;
}

//==============================================================================
// Set Checking Index
//==============================================================================
void MainWindow::setCheckIndex(const int& aIndex)
{
    // Check If Checking Active
    if (mCheckingActive && mCheckingIndex != aIndex) {
        qDebug() << "MainWindow::setCheckIndex - aIndex: " << aIndex;
        // Set Checking Index
        mCheckingIndex = aIndex;

        // Emit Checking Index Changed Signal
        emit checkIndexChanged(mCheckingIndex);

        // Show Status Message
        showStatusMessage("Checking Translation...", -1);
    }
}

//==============================================================================
// Get Cell Edit Mode
//==============================================================================
bool MainWindow::cellEditMode()
{
    return mCellEditMode;
}

//==============================================================================
// Set Cell Edit Mode
//==============================================================================
void MainWindow::setCellEditMode(const bool& aCellEditMode)
{
    // Check Cell Edit Mode
    if (mCellEditMode != aCellEditMode) {
        // Set Cell Edit Mode
        mCellEditMode = aCellEditMode;
        // Emit Cell Edit Mode Changed Signal
        emit cellEditModeChanged(mCellEditMode);

        // Show Status Message
        showStatusMessage(mCellEditMode ? "Edit Mode On." : "Edif Mode Off.");
    }
}

//==============================================================================
// Get Busy State
//==============================================================================
bool MainWindow::busy()
{
    return mBusy;
}

//==============================================================================
// Set Busy State
//==============================================================================
void MainWindow::setBusy(const bool& aBusy)
{
    // Check Busy State
    if (mBusy != aBusy) {
        // Set Busy State
        mBusy = aBusy;
        // Emit Busy State changed Signal
        emit busyChanged(mBusy);

        // Check Busy State
        if (!mBusy) {
            // Emit Busy Text Changed Signal to Reset Busy Text
            emit busyTextChanged("Busy...");
        }
    }
}

//==============================================================================
// Get Loaders Count
//==============================================================================
int MainWindow::loadersCount()
{
    return mLoadersCount;
}

//==============================================================================
// Set Loaders Count
//==============================================================================
void MainWindow::setLoadersCount(const int& aCount)
{
    // Check Loaders Count
    if (mLoadersCount != aCount) {
        qDebug() << "MainWindow::setLoadersCount - aCount: " << aCount;
        // Set Loaders Count
        mLoadersCount = aCount;
        // Emit Loaders Count Changed Signal
        emit loadersCountChanged(mLoadersCount);
    }
}

//==============================================================================
// Get Language Name
//==============================================================================
QString MainWindow::getLanguageName(const QString& aLocale)
{
    return mLanguageMap[aLocale];
}

//==============================================================================
// Get Language Flag URL
//==============================================================================
QString MainWindow::getLanguageFlag(const QString& aLocale)
{
    // resources/images/icons/flags/en-US.ico
    return QString("qrc:/resources/images/flags/%1.png").arg(aLocale.trimmed());
}

//==============================================================================
// Show Info Dialog
//==============================================================================
void MainWindow::showInfoDialog(const QString& aTitle, const QString& aInfoText)
{
    // Check Info Dialog
    if (!mInfoDialog) {
        // Create Info Dialog
        mInfoDialog = new InfoDialog(this);
    }

    // Set Title
    mInfoDialog->setTitle(aTitle);
    // Set Info Text
    mInfoDialog->setInfoText(aInfoText);

    // Exec Dialog
    mInfoDialog->exec();
}

//==============================================================================
// Get Confirmation
//==============================================================================
bool MainWindow::getConfirmation(const QString& aTitle, const QString& aConfirmText)
{
    qDebug() << "MainWindow::getConfirmation - aTitle: " << aTitle << " - aConfirmText: " << aConfirmText;

    // Check Confirmation Dialog
    if (!mConfirmDialog) {
        // Create Confirm Dialog
        mConfirmDialog = new ConfirmationDialog(this);
    }

    // Set Title
    mConfirmDialog->setTitle(aTitle);
    // Set Confirmation Text
    mConfirmDialog->setConfirmText(aConfirmText);

    // Exec Confirm Dialog
    return mConfirmDialog->exec();
}

//==============================================================================
// Create New Package/Project
//==============================================================================
void MainWindow::createNewPackage()
{
    // Close Package
    closePackage();

    qDebug() << "MainWindow::createNewPackage";

    // Launch Package Preferences Dialog
    launchPackagePreferencesDialog(true);
}

//==============================================================================
// Open Package
//==============================================================================
void MainWindow::loadPackage(const QString& aFilePath)
{
    // Close Package
    closePackage();

    // Init Package File Info
    QFileInfo packageFileInfo(aFilePath);

    // Check Package File Info
    if (packageFileInfo.exists()) {
        // Set Current Path
        mCurrentPath = packageFileInfo.absoluteFilePath();
        // Add Recent File
        addRecentFile(mCurrentPath);

        // Create Package
        PackageInfo* newPackage = new PackageInfo();
        // Load Package
        newPackage->loadPackage(mCurrentPath);

        // Set Current Package
        setCurrentPackage(newPackage);

        // Show Status Message
        showStatusMessage("Project Loaded.");

        // Check If Preview Was Shown
        if (mPreviewWasShown) {
            // Reset Preview Was Shown
            mPreviewWasShown = false;
            // Show Preview Pane
            showPreviewPane();
        }
    }
}

//==============================================================================
// Load Package
//==============================================================================
void MainWindow::loadPackage()
{
//    // Close Package
//    closePackage();

    // Init Initial Path
    QString initialPath = mCurrentPath.isEmpty() ? QDir::homePath() : mCurrentPath;
    // Get File Path
    QString openFilePath = QFileDialog::getOpenFileName(nullptr, "Open Package", initialPath, QString("JSON Files (*.json)"));

    // Check New File Path
    if (!openFilePath.isEmpty()) {
        qDebug() << "MainWindow::loadPackage - openFilePath: " << openFilePath;

        // Load Package
        loadPackage(openFilePath);
    }
}

//==============================================================================
// Save Package
//==============================================================================
void MainWindow::savePackage()
{
    // Check Current Package
    if (mCurrentPackage) {
        // Check Path
        if (mCurrentPackage->path().isEmpty()) {
            // Init Initial Path
            QString initialPath = QDir::homePath();
            // Get Save File Name
            QString saveFileName = QFileDialog::getSaveFileName(nullptr, "Save Package", initialPath, QString("JSON Files (*.json)"));

            // Check Save File Name
            if (!saveFileName.isEmpty()) {
                // Check Save File Name
                if (!saveFileName.endsWith(DEFAULT_PACKAGE_FILENAME_SUFFIX)) {
                    // Append Suffix
                    saveFileName += DEFAULT_PACKAGE_FILENAME_SUFFIX;
                }

                // Check If File Exists
                if (QFile::exists(saveFileName)) {
                    // Get Confirmation
                    if (getConfirmation("Confirmation", "Package File Exists. Overwrite?")) {
                        // Save Package
                        mCurrentPackage->savePackage(saveFileName);
                    }
                } else {
                    // Save Package
                    mCurrentPackage->savePackage(saveFileName);
                }

                // Add Recent File
                addRecentFile(saveFileName);
            }
        } else {
            // Save Package
            mCurrentPackage->savePackage();
        }

        // Show Status Message
        showStatusMessage("Project Saved.");

        // Update Menu
        updateMenu();
        // Update Window Title
        updateWindowTitle();
    }
}

//==============================================================================
// Save Package As
//==============================================================================
void MainWindow::saveAsPackage()
{
    // Check Current Package
    if (mCurrentPackage) {
        // Init Initial Path
        QString initialPath = mCurrentPackage->path().isEmpty() ? QDir::homePath() : QFileInfo(mCurrentPackage->path()).absolutePath();
        // Get New Path
        QString newPath = QFileDialog::getSaveFileName(nullptr, "Save Package As", initialPath, QString("JSON Files (*.json)"));

        // Check New Path
        if (!newPath.isEmpty()) {
            // Save Package
            mCurrentPackage->savePackage(newPath);
        }

        // Show Status Message
        showStatusMessage("Project Saved.");

        // Update Menu
        updateMenu();
        // Update Window Title
        updateWindowTitle();
    }
}

//==============================================================================
// Close Package
//==============================================================================
void MainWindow::closePackage()
{
    // Check Current Package
    if (mCurrentPackage) {
        // Hide Preview Pane
        hidePreviewPane();

        qDebug() << "MainWindow::closePackage";

        // Reset Cell Edit Mode
        setCellEditMode(false);

        // Check If Dirty
        if (mCurrentPackage->isDirty()) {
            // Ask For Save Confirmation
            if (getConfirmation("Confirmation", "Current Package Not Saved, Save?")) {
                // Save Package
                savePackage();
            }
        }

        // Disconnect Signals
        disconnect(mCurrentPackage, SIGNAL(dirtyChanged(bool)), this, SLOT(currentPackageDirtyChanged(bool)));
        disconnect(mCurrentPackage, SIGNAL(itemAboutToBeRemoved(int)), this, SLOT(itemAboutToBeRemoved(int)));
        disconnect(mCurrentPackage, SIGNAL(itemRemoved(int)), this, SLOT(itemRemoved(int)));
        disconnect(mCurrentPackage, SIGNAL(languagesChanged(QStringList)), this, SLOT(packageLanguagesChanged(QStringList)));

        // ...

        // Reset Current Layout
        setCurrentLayout(nullptr);
        // Reset Current Item
        setCurrentItem(nullptr);
        // reset Current Group
        setCurrentGroup(nullptr);

        // Delete Current Package
        delete mCurrentPackage;

        // Reset Current Package
        setCurrentPackage(nullptr);

        // Show Status Message
        showStatusMessage("Project Closed.");

        // ...
    }
}

//==============================================================================
// Set Current Package
//==============================================================================
void MainWindow::setCurrentPackage(PackageInfo* aPackage)
{
    // Check Current Package
    if (mCurrentPackage != aPackage) {
        qDebug() << "MainWindow::setCurrentPackage - aPackage: " << (aPackage ? aPackage->packageName() : "NULL");

//        // Check Current Package
//        if (mCurrentPackage) {
//            // Disconnect Signals
//            disconnect(mCurrentPackage, SIGNAL(dirtyChanged(bool)), this, SLOT(currentPackageDirtyChanged(bool)));

//            // ...
//        }

        // Set Current Package
        mCurrentPackage = aPackage;
        // Emit Current Package changed Signal
        emit currentPackageChanged(mCurrentPackage);

        // Check Current Package
        if (mCurrentPackage) {
            // Connect Signals
            connect(mCurrentPackage, SIGNAL(dirtyChanged(bool)), this, SLOT(currentPackageDirtyChanged(bool)));
            connect(mCurrentPackage, SIGNAL(itemAboutToBeRemoved(int)), this, SLOT(itemAboutToBeRemoved(int)));
            connect(mCurrentPackage, SIGNAL(itemRemoved(int)), this, SLOT(itemRemoved(int)));
            connect(mCurrentPackage, SIGNAL(languagesChanged(QStringList)), this, SLOT(packageLanguagesChanged(QStringList)));

            // ...
        } else {

            // Reset Currenct Group
            setCurrentGroup(nullptr);
            // Reset Current Translation Item
            setCurrentItem(nullptr);
            // Reset Current Layout
            setCurrentLayout(nullptr);

            // ...
        }

        // Check Find Items Dialog
        if (mFindItemsDialog) {
            // Set Package
            mFindItemsDialog->setPackage(mCurrentPackage);
        }

        // ...

        // Update Menu
        updateMenu();
        // Update Window Title
        updateWindowTitle();
        // Update Translation Language Selector
        updateTranslationLanguageSelector();
        // Update Translation Font Selector
        updateTranslationFontSelector();
    }
}

//==============================================================================
// Set Current Group
//==============================================================================
void MainWindow::setCurrentGroup(PackageGroup* aPackageGroup)
{
    // Check Current Group
    if (mCurrentGroup != aPackageGroup) {
        // Set Current Group
        mCurrentGroup = aPackageGroup;
        // Emit Current Group Changed Signal
        emit currentGroupChanged(mCurrentGroup);
    }
}

//==============================================================================
// Set Current Index
//==============================================================================
void MainWindow::setCurrentIndex(const int& aIndex)
{
    // Check Current Index
    if (mCurrentIndex != aIndex) {
        //qDebug() << "MainWindow::setCurrentIndex - aIndex: " << aIndex;
        // Set Current Index
        mCurrentIndex = aIndex;
        // Emit Current Index Changed Signal
        emit currentIndexChanged(mCurrentIndex);
    }
}

//==============================================================================
// Set Current Translation Item
//==============================================================================
void MainWindow::setCurrentItem(TranslationItem* aTranslationItem)
{
    // Check Current Item
    if (mCurrentItem != aTranslationItem) {
        //qDebug() << "MainWindow::setCurrentItem - aTranslationItem: " << (aTranslationItem ? aTranslationItem->textID() : "NULL");

        // Set Current Item
        mCurrentItem = aTranslationItem;
        // Emit Current Item Changed Signal
        emit currentItemChanged(mCurrentItem);

        // Check Current Item
        if (mCurrentItem) {
            // Set Current Layout
            setCurrentLayout(mCurrentItem->currentLayout());
        } else {
            // Reset Current Layout
            setCurrentLayout(nullptr);
        }

        // Update Menu Item
        ui->actionShowHidePreview->setEnabled(mCurrentItem);
    }
}

//==============================================================================
// Set Current Layout
//==============================================================================
void MainWindow::setCurrentLayout(TranslationItemLayout* aTranslationItemLayout)
{
    // Check Current Layout
    if (mCurrentLayout != aTranslationItemLayout) {
        // Set Current Layout
        mCurrentLayout = aTranslationItemLayout;

        // Emit Current Layout Changed Signal
        emit currentLayoutChanged(mCurrentLayout);

        // Update Preview Pane
        updatePreviewPane();

        // Update Font Selector Combo Box
    }
}

//==============================================================================
// Set Selected Translation Item Language
//==============================================================================
void MainWindow::setSelectedItemLanguage(const QString& aLanguage)
{
    // Check Selected Item Language
    if (mSelectedItemLanguage != aLanguage) {
        qDebug() << "MainWindow::setSelectedItemLanguage - aLanguage: " << aLanguage;
        // Set Selected Item Language
        mSelectedItemLanguage = aLanguage;
        // Emit Selected Item Language Changed Signal
        emit selectedItemLanguageChanged(mSelectedItemLanguage);
    }
}

//==============================================================================
// Select All Translations
//==============================================================================
void MainWindow::selectAllTranslations()
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::selectAllTranslations";

        // Select All Translations
        mCurrentPackage->selectAllItems();
    }
}

//==============================================================================
// Select None Translations
//==============================================================================
void MainWindow::selectNoneTranslations()
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::selectNoneTranslations";

        // Select No Items
        mCurrentPackage->selectNoItems();
    }
}

//==============================================================================
// Add Translation Group
//==============================================================================
void MainWindow::addTranslationGroup()
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::addTranslationGroup";

        // Check New Group Dialog
        if (!mGroupDialog) {
            // Create New Group Dialog
            mGroupDialog = new NewGroupDialog(this);
        }

        // Exec Group Dialog
        if (mGroupDialog->exec()) {
            // Get New Group Name
            QString newGroupName = mGroupDialog->groupName();

            // Add Group
            mCurrentPackage->addGroup(newGroupName);

            // Update Menu
            updateMenu();

        }

        // ...
    }
}

//==============================================================================
// Remove Translation Group
//==============================================================================
void MainWindow::removeTranslationGroup(const int& aIndex)
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::removeTranslationGroup - aIndex: " << aIndex;

        // ...
    }
}

//==============================================================================
// Select Translation Item
//==============================================================================
TranslationItem* MainWindow::selectTranslationItem(const int& aIndex)
{
    // Check Current Package
    if (mCurrentPackage) {
        //qDebug() << "MainWindow::selectTranslationItem - aIndex: " << aIndex;

        // Set Current Translation Item
        setCurrentItem(mCurrentPackage->getItem(aIndex));
        // Set Current Index
        setCurrentIndex(mCurrentItem ? aIndex : -1);

        return mCurrentItem;
    }

    return nullptr;
}

//==============================================================================
// Edit Translation Item
//==============================================================================
void MainWindow::editTranslationItem(const int& aIndex)
{
    // Check Index
    if (aIndex != -1) {
        // Select Translation Item
        selectTranslationItem(aIndex);
    }

    qDebug() << "MainWindow::editTranslationItem - aIndex: " << aIndex;

    // Check Translation Item Dialog
    if (!mTranslationDialog) {
        // Create Translation Item Dialog
        mTranslationDialog = new NewTranslationDialog(this, this);
    }

    // Set Current Package
    mTranslationDialog->setCurrentPackage(mCurrentPackage);

    // Set Group Selection Enabled
    mTranslationDialog->setGroupSelectorEnabled(false);

    // Load Translation Item
    mTranslationDialog->loadTranslationItem(mCurrentItem);
    // Set Window Title
    mTranslationDialog->setWindowTitle("Edit Translation Item");

    // Exec Translation Item Dialog
    if (mTranslationDialog->exec()) {
        // Save Translation Item
        mTranslationDialog->saveTranslationItem();
        // Reset Current Translation Item
        mTranslationDialog->loadTranslationItem(nullptr);

        // Check Current Package
        if (mCurrentPackage) {
            // Notify Current Package
            mCurrentPackage->refresh(aIndex);
        }
    }
}

//==============================================================================
// Add Translation Item
//==============================================================================
void MainWindow::addTranslationItem()
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::addTranslationItem";

        // Check New Translation Item Dialog
        if (!mTranslationDialog) {
            // Create New Translation Item Dialog
            mTranslationDialog = new NewTranslationDialog(this, this);
        }

        // Set Current Package
        mTranslationDialog->setCurrentPackage(mCurrentPackage);
        // Set Group Selection Enabled
        mTranslationDialog->setGroupSelectorEnabled(true);

        // Create New Translation Item
        TranslationItem* newTranslationItem = new TranslationItem(mCurrentPackage, "");
        // Append Default Layout
        newTranslationItem->addLayout(true);

        // Load Translation Item
        mTranslationDialog->loadTranslationItem(newTranslationItem);
        // Set Window Title
        mTranslationDialog->setWindowTitle("Create New Translation Item");

        // Set Focus
        mTranslationDialog->setTextIDEditorForcused();

        // Exec Translation Item Dialog
        if (mTranslationDialog->exec()) {
            // Save Translation Item
            mTranslationDialog->saveTranslationItem();
            // Reset Current Translation Item
            mTranslationDialog->loadTranslationItem(nullptr);

            // Add Translation Item
            if (!mCurrentPackage->addTranslationItem(newTranslationItem->translationGroup(), newTranslationItem)) {
                // Discard New Translation Item
                delete newTranslationItem;
            }
        } else {
            // Reset Current Translation Item
            mTranslationDialog->loadTranslationItem(nullptr);
            // Discard New Translation Item
            delete newTranslationItem;
        }

        // Reset Current Package
        mTranslationDialog->setCurrentPackage(nullptr);

        // ...
    }
}

//==============================================================================
// Remove Translation Item
//==============================================================================
void MainWindow::removeTranslationItem(const int& aIndex)
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::removeTranslationItem - aIndex: " << aIndex;

//        // Check Current Index
//        if (aIndex == -1) {
//            // Select Translation Item
//            selectTranslationItem(mCurrentIndex - 1);
//            // Reset Current Item
//            //setCurrentItem(nullptr);
//        }

        // Remove Translation Item
        mCurrentPackage->removeTranslationItem(aIndex == -1 ? mCurrentIndex : aIndex);
    }
}

//==============================================================================
// Add Translation Layout
//==============================================================================
void MainWindow::addTranslationLayout()
{
    // Chek Current Translation Item
    if (mCurrentItem) {
        qDebug() << "MainWindow::addTranslationLayout";

        // Add Translation Layout
        mCurrentItem->addLayout();
        // Set Layout Index
        mCurrentItem->setLayoutIndex(mCurrentItem->layoutCount() - 1);

        // Set Current Layout
        setCurrentLayout(mCurrentItem->currentLayout());

//        // Update Preview Pane
//        updatePreviewPane();
    }
}

//==============================================================================
// Remvoe Translation Layout
//==============================================================================
void MainWindow::removeTranslationLayout(const int& aIndex)
{
    // Chek Current Translation Item
    if (mCurrentItem) {
        qDebug() << "MainWindow::removeTranslationLayout - aIndex: " << aIndex;
        // Remove Translation Layout
        mCurrentItem->removeLayout(aIndex == -1 ? mCurrentItem->layoutIndex() : aIndex);

        // Set Current Layout
        setCurrentLayout(mCurrentItem->currentLayout());

//        // Update Preview Pane
//        updatePreviewPane();
    }
}

//==============================================================================
// Goto Previous Translation Layout
//==============================================================================
void MainWindow::prevTranslationLayout()
{
    // Check Current Item
    if (mCurrentItem) {
        // Prev Layout
        mCurrentItem->prevLayout();

        // Set Current Layout
        setCurrentLayout(mCurrentItem->currentLayout());

        // Update Preview Pane
        //updatePreviewPane();
    }
}

//==============================================================================
// Goto Next Translation Layout
//==============================================================================
void MainWindow::nextTranslationLayout()
{
    // Check Current Item
    if (mCurrentItem) {
        // Next Layout
        mCurrentItem->nextLayout();

        // Set Current Layout
        setCurrentLayout(mCurrentItem->currentLayout());

        // Update Preview Pane
        //updatePreviewPane();
    }
}

//==============================================================================
// Check Current Translation Item
//==============================================================================
void MainWindow::checkTranslationItem()
{
    qDebug() << "MainWindow::checkTranslationItem";

    // Save Last Index
    mLastIndex = mCurrentIndex;
    // Save Selected Item Language
    mPreservedItemLanguage = ui->languageComboBox->currentIndex();

    // Set Action Disabled
    ui->actionCheckTranslation->setEnabled(false);
    ui->actionCheckAllTranslations->setEnabled(false);

    // Reset Cell Edit Mode
    setCellEditMode(false);

    // Set Checking Active
    setCheckingActive(true);

    // Reset Check Index
    setCheckIndex(mCurrentIndex);

    // Show Preview Pane
    showPreviewPane();

    // Check Current Package
    if (mCurrentPackage && mCurrentPackage->selectedItemCount() > 0) {
        // Emit Checking Started Signal
        emit checkingStarted();
    } else {
        // Emit Single Check Started Signal
        emit singleCheckStarted();
    }

    // Show Status Message
    showStatusMessage("Check Started.");

    // ...
}

//==============================================================================
// Check All Translations
//==============================================================================
void MainWindow::checkAllTranslations()
{
    // Check If Checking Is Already Active
    if (!mCheckingActive && mCurrentPackage) {
        qDebug() << "MainWindow::checkAllTranslations";

        // Save Last Index
        mLastIndex = mCurrentIndex;

        // Save Selected Item Language
        mPreservedItemLanguage = ui->languageComboBox->currentIndex();

        // Set Action Disabled
        ui->actionCheckAllTranslations->setEnabled(false);

        // Reset Cell Edit Mode
        setCellEditMode(false);

        // Set Checking Active
        setCheckingActive(true);

        // Emit Reset Sorting Signal
        emit resetSorting();

        // Select No Translations
        selectNoneTranslations();
        // Select All Groups
        mCurrentPackage->selectAllGroups();
        // Clear All Filters
        mCurrentPackage->setFilterText("");
        // Select All Languages
        mCurrentPackage->setAllLanguagesSelected(true);

        // Reset Check Index
        setCheckIndex(-1);

        // Show Preview Pane
        showPreviewPane();

        // Emit Checking Started Signal
        emit checkingStarted();

        // Show Status Message
        showStatusMessage("Check Started.");

//        // Reset Current Item
//        setCurrentItem(nullptr);
//        // Select 1st Translation Item
//        selectTranslationItem(0);

        // ...

    } else {
        // Set Action Disabled
        ui->actionCheckAllTranslations->setEnabled(mCurrentPackage);
        // Reset Checking Index
        setCheckIndex(-1);
    }
}

//==============================================================================
// Stop Checking
//==============================================================================
void MainWindow::stopCheckingAllTranslations(const bool& aAbort)
{
    // Check If Checkign Mode Active
    if (mCheckingActive) {
        qDebug() << "MainWindow::stopCheckingAllTranslations - aAbort: " << aAbort;

        // Check Abort
        if (aAbort) {
            // Emit Checking Aborted Signal
            emit checkingAborted();
        }

        // Set Checking Active
        setCheckingActive(false);

        // Reset Check Index
        mCheckingIndex = -1;

        // Set Action Enabled State
        ui->actionCheckAllTranslations->setEnabled(mCurrentPackage);
        ui->actionCheckTranslation->setEnabled(mCurrentItem);

        // Check Preserved Item Language
        if (mPreservedItemLanguage != -1) {
            // Set Selected Item Language
            setSelectedTranslationLanguage(mPreservedItemLanguage, true);
            // Reset Preserved Item Language
            mPreservedItemLanguage = -1;
        }

//        // Show Status Message
//        showStatusMessage("Checking Translation Stopped.");

        // Update Preview Pane
        updatePreviewPane();

        // ...

    }
}

//==============================================================================
// Finish Checking
//==============================================================================
void MainWindow::finishCheckingAllTranslations(const bool& aSingleCheck)
{
    qDebug() << "MainWindow::finishCheckingAllTranslations";

    // Select None
    //selectNoneTranslations();

    // Stop Checking All Translations
    stopCheckingAllTranslations();

    // Check Last Index
    if (mLastIndex != -1) {
        // Re Select Translation Item
        selectTranslationItem(mLastIndex);
        // Reset Last Index
        mLastIndex = -1;
    }

    // Emit Checking Finished Signal
    emit checkingFinished(aSingleCheck);

    // Show Status Message
    showStatusMessage("Check Finished.");

    // ...
}

//==============================================================================
// Show Status Message
//==============================================================================
void MainWindow::showStatusMessage(const QString& aMessage, const int& aTimeOut)
{
    // Show Status Bar Message
    ui->statusBar->showMessage(aMessage, aTimeOut);
}

//==============================================================================
// Set Selected Translation Language
//==============================================================================
void MainWindow::setSelectedTranslationLanguage(const int& aIndex, const bool& aUpdateComboBox)
{
    // Check Current Package
    if (mCurrentPackage && aIndex >= 0 && aIndex < mCurrentPackage->languages().count()) {
        //qDebug() << "MainWindow::setSelectedTranslationLanguage - aIndex: " << aIndex << " - aUpdateComboBox: " << aUpdateComboBox;
        // Get Language
        QString language = mCurrentPackage->languages()[aIndex];

        // Save Last Language
        mPreferences->setLastLanguage(language);

        // Set Selected Item Language
        setSelectedItemLanguage(language);
    }

    // Check Update ComboBox
    if (aUpdateComboBox) {
        // Set Current Index
        ui->languageComboBox->setCurrentIndex(aIndex);
    }
}

//==============================================================================
// Get Selected Language Index
//==============================================================================
int MainWindow::getSelectedLanguageIndex()
{
    // Check Current Package
    if (mCurrentPackage) {
        return mCurrentPackage->languages().indexOf(mSelectedItemLanguage);
    }

    return -1;
}

//==============================================================================
// Set Current Translation Item Current Layout Max Width
//==============================================================================
void MainWindow::setLayoutMaxWidth(const int& aWidth)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Max Width
        mCurrentLayout->setMaxWidth(aWidth);
    }

    // Update Preview Pane
    updatePreviewPane();
}

//==============================================================================
// Set Current Translation Item Current Layout Max Height
//==============================================================================
void MainWindow::setLayoutMaxHeight(const int& aHeight)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Max Height
        mCurrentLayout->setMaxHeight(aHeight);
    }

    // Update Preview Pane
    updatePreviewPane();
}

//==============================================================================
// Set Current Translation Item Current Layout Max Lines
//==============================================================================
void MainWindow::setLayoutMaxLines(const int& aLines)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Max Lines
        mCurrentLayout->setMaxLines(aLines);
    }

    // Update Preview Pane
    updatePreviewPane();
}

//==============================================================================
// Set Current Translation Item Max Length
//==============================================================================
void MainWindow::setLayoutMaxLength(const int& aMaxLength)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Max Length
        mCurrentLayout->setMaxLength(aMaxLength);
    }

    // Update Preview Pane
    updatePreviewPane();
}

//==============================================================================
// Set Current Translation Item Current Layout Wrap Type
//==============================================================================
void MainWindow::setLayoutWrapType(const int& aWrapType)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Wrap Type
        mCurrentLayout->setWrapType(aWrapType);
    }
}

//==============================================================================
// Set Current Translation Item Current Layout Elide Type
//==============================================================================
void MainWindow::setLayoutElideType(const int& aElideType)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Elide Type
        mCurrentLayout->setElideType(aElideType);
    }
}

//==============================================================================
// Set Current Translation Item Current Layout Font Name Index
//==============================================================================
void MainWindow::setLayoutFontName(const int& aFontIndex)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Font Name
        mCurrentLayout->setFontName(ui->fontComboBox->itemText(aFontIndex));
    }
}

//==============================================================================
// Set Current Translation Item Current Layout Font Size
//==============================================================================
void MainWindow::setLayoutFontSize(const int& aFontSize)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Font Size
        mCurrentLayout->setFontSize(aFontSize);
    }
}

//==============================================================================
// Set Layout Shrinkable
//==============================================================================
void MainWindow::setLayoutShrinkable(const bool& aShrinkable)
{
    // Check Current Layout
    if (mCurrentLayout) {
        // Set Shrinkable
        mCurrentLayout->setShrinkable(aShrinkable);
    }
}

//==============================================================================
// Launch Find Dialog
//==============================================================================
void MainWindow::launchFindDialog()
{
    // Check Current Package
    if (!mCurrentPackage) {
        return;
    }

    // Check Find Dialog
    if (!mFindItemsDialog) {
        // Create Find Items Dialog
        mFindItemsDialog = new FindItemsDialog(mCurrentPackage, this);
        // Connect Signal
        connect(mFindItemsDialog, &FindItemsDialog::resultItemSelected, this, &MainWindow::searchResultItemSelected);
    }

    // Set Current Package
    mFindItemsDialog->setPackage(mCurrentPackage);
    // Launch Find Dialog
    mFindItemsDialog->show();

}

//==============================================================================
// Launch Package Preferences
//==============================================================================
void MainWindow::launchPackagePreferencesDialog(const bool& aNewPackage)
{
    // Check Package Preferences Dialog
    if (!mPackagePreferencesDialog) {
        // Create Package Preferences Dialog
        mPackagePreferencesDialog = new PackagePreferencesDialog(this);
    }

    // Check New Package
    if (aNewPackage) {
        // Set Title
        mPackagePreferencesDialog->setWindowTitle("Create New Package");
        // Load Defaults
        mPackagePreferencesDialog->loadDefaults();
        // Create New Package Info & Set As Current Package
        mPackagePreferencesDialog->setCurrentPackage(new PackageInfo());
    } else {
        // Set Title
        mPackagePreferencesDialog->setWindowTitle("Package Preferences");
        // Load Package Preferences
        mPackagePreferencesDialog->loadPreferences(mCurrentPackage);
    }

    // Launch Package Preferences Dialog
    if (mPackagePreferencesDialog->exec()) {
        // Check New Package
        if (aNewPackage) {
            // Set Current Package
            setCurrentPackage(mPackagePreferencesDialog->currentPackage());

            // Show Status Message
            showStatusMessage("Project Created.");
        }

        // Save Package Preferences
        mPackagePreferencesDialog->savePreferences();

        // Update Translation Font Selector
        updateTranslationFontSelector();

    } else {
        // Check New Package
        if (aNewPackage) {
            // Discard New Package
            mPackagePreferencesDialog->discardNewPackage();
        }
    }
}

//==============================================================================
// Launch Alternative Fonts Dialog
//==============================================================================
void MainWindow::launchAlternativeFontsDialog()
{
    // Check Current Layout
    if (mCurrentLayout) {
        qDebug() << "MainWindow::launchAlternativeFontsDialog";

        // Check Dialog
        if (!mAlternativeFontsDialog) {
            // Create Dialog
            mAlternativeFontsDialog = new AlternateFontsDialog(mCurrentPackage, this);
        }

        // Set Current Package
        mAlternativeFontsDialog->setCurrentPackage(mCurrentPackage);

        // Set Selected Font List
        mAlternativeFontsDialog->setSelectedFonts(mCurrentLayout->altFonts(), mCurrentLayout->fontName());

        // Get Main Window Geometry
        QRect mainRect = geometry();
        // Get Dialog Geometry
        QRect dialogRect = mAlternativeFontsDialog->geometry();

        // Set Dialog Geometry
        mAlternativeFontsDialog->setGeometry(mainRect.right() - dialogRect.width() - 32, mainRect.center().y() + 64, dialogRect.width(), dialogRect.height());

        // Exec Dialog
        if (mAlternativeFontsDialog->exec()) {
            // Save Alternative Fonts
            mCurrentLayout->setAltFonts(mAlternativeFontsDialog->selectedFonts());
            // Set Alternative Fonts Label
            ui->alternativeFontsLabel->setText(mCurrentLayout->altFonts().isEmpty() ? "" : mCurrentLayout->altFonts().join(";"));
        }
    }
}

//==============================================================================
// Get Last Window State
//==============================================================================
int MainWindow::lastWindowState()
{
    return mPreferences->lastWindowState();
}

//==============================================================================
// Show Preview Pane
//==============================================================================
void MainWindow::showPreviewPane()
{
    //qDebug() << "MainWindow::showPreviewPane - mLastPreviewWidth: " << mLastPreviewWidth;

    // Get Sizes
    QList<int> sizes = ui->mainSplitter->sizes();
    // Set Size
    sizes[1] = mLastPreviewWidth == 0 ? SETTINGS_DEFAULT_DEFAULT_PREVIEW_WIDTH : mLastPreviewWidth;
    // Set Size For Main View
    sizes[0] = geometry().width() - sizes[1] - 24;
    // Set Sizes
    ui->mainSplitter->setSizes(sizes);
}

//==============================================================================
// Hide Preview Pane
//==============================================================================
void MainWindow::hidePreviewPane()
{
    // Check Main Splitter Sizes
    if (ui->mainSplitter->sizes()[1] != 0) {
        // Set Preview Was Shown
        mPreviewWasShown = true;
        // Set Last Preview Width
        mLastPreviewWidth = ui->mainSplitter->sizes()[1];

        //qDebug() << "MainWindow::hidePreviewPane - mLastPreviewWidth: " << mLastPreviewWidth;

        // Get Sizes
        QList<int> sizes = ui->mainSplitter->sizes();
        // Set Size
        sizes[1] = 0;
        // Set Sizes
        ui->mainSplitter->setSizes(sizes);
    }
}

//==============================================================================
// Show Hide Preview
//==============================================================================
void MainWindow::showHidePreview()
{
    // Check Main Splitter Sizes
    if (ui->mainSplitter->sizes()[1] != 0) {
        // Hide Preview Pane
        hidePreviewPane();
    } else {
        // Show Preview Pane
        showPreviewPane();
    }
}

//==============================================================================
// Add Recent File
//==============================================================================
void MainWindow::addRecentFile(const QString& aPath)
{
    // Check Recent Files
    if (QFile::exists(aPath)) {
        qDebug() << "MainWindow::addRecentFile - aPath: " << aPath;

        // Get Index
        int rfIndex = mRecentFiles.indexOf(aPath);
        // Check Index
        if (rfIndex != -1) {
            // Remove Item
            mRecentFiles.removeAt(rfIndex);
        }

        // Add Recent File
        mRecentFiles.insert(0, aPath);

        // Check Count
        if (mRecentFiles.count() > DEFAULT_RECENT_FILES_MAX) {
            // Remove Last
            mRecentFiles.removeLast();
        }

        // Emit Recent Files Changed Signal
        emit recentFilesChanged(mRecentFiles);
    }
}

//==============================================================================
// Set Checking Active
//==============================================================================
void MainWindow::setCheckingActive(const bool& aCheckingActive)
{
    // Check Checking Active
    if (mCheckingActive != aCheckingActive) {
        qDebug() << "MainWindow::setCheckingActive - aCheckingActive: " << aCheckingActive;
        // Set Checking Active
        mCheckingActive = aCheckingActive;
        // Emit Checking Active Changed Signal
        emit checkingActiveChanged(mCheckingActive);
    }
}

//==============================================================================
// Load Recent Files List
//==============================================================================
void MainWindow::loadRecentFilesList()
{
    // Clear Recent Files
    mRecentFiles.clear();

    qDebug() << "MainWindow::loadRecentFilesList";

    // Init Recent Files File Path
    QString recentFilesPath = QDir::homePath() + "/" + DEFAULT_RECENT_FILES_NAME;

    // Init Recent Files
    QFile recentFiles(recentFilesPath);

    // Check If File Exists
    if (recentFiles.exists()) {
        // Open Recent Files
        if (recentFiles.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Init Text Stream
            QTextStream readStream(&recentFiles);
            // Read All And Set Recent Files
            mRecentFiles = readStream.readAll().split("\n");
            // Close Recent Files
            recentFiles.close();
        }
    }

    // ...

    // Emit Recent Files Changed Signal
    emit recentFilesChanged(mRecentFiles);
}

//==============================================================================
// Save Recent Files List
//==============================================================================
void MainWindow::saveRecentFilesList()
{
    qDebug() << "MainWindow::saveRecentFilesList";

    // Init Recent Files File Path
    QString recentFilesPath = QDir::homePath() + "/" + DEFAULT_RECENT_FILES_NAME;

    // Init Recent Files
    QFile recentFiles(recentFilesPath);

    // Check Recent Files
    if (mRecentFiles.count() > 0) {
        // Open Recent Files
        if (recentFiles.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Init Write Stream
            QTextStream writeStream(&recentFiles);
            // Write to Stream
            writeStream << mRecentFiles.join('\n');
            // Flush
            writeStream.flush();
            // Close File
            recentFiles.close();
        }
    } else {
        // Remove Recent Files
        recentFiles.remove();
    }
}

//==============================================================================
// Clear Recent Files List
//==============================================================================
void MainWindow::clearRecentFilesList()
{
    //Check Recent Files
    if (mRecentFiles.count() > 0) {
        qDebug() << "MainWindow::clearRecentFilesList";

        // Clear Recent Files
        mRecentFiles.clear();
        // Emit Recent Files Changed
        emit recentFilesChanged(mRecentFiles);
    }
}

//==============================================================================
// Launch Preferences
//==============================================================================
void MainWindow::launchPreferencesDialog()
{
    // Check Preferences Dialog
    if (!mPreferencesDialog) {
        // Create Preferences Dialog
        mPreferencesDialog = new PreferencesDialog(this);
    } else {
        // Load Preferences
        mPreferencesDialog->loadPreferences();
    }

    // Exec Preferences Dialog
    if (mPreferencesDialog->exec()) {
        // ...
    }

    // Clear Plugins
    clearPlugins();
    // Load Plugins
    loadPlugins();

    // Update Import/Export Submenu's
    updateImportExportSubMenu();
    // Update Menu
    updateMenu();
}

//==============================================================================
// Launch About
//==============================================================================
void MainWindow::launchAboutDialog()
{
    // Check About Dialog
    if (!mAboutDialog) {
        // Create About Dialog
        mAboutDialog = new AboutDialog(this);
    }

    // Exec
    mAboutDialog->exec();
}

//==============================================================================
// Launch Help
//==============================================================================
void MainWindow::launchHelpDialog()
{
    // Check Help Dialog
    if (!mHelpDialog) {
        // Create Help Dialog
        mHelpDialog = new HelpDialog(this);
    }

    // Show Help Dialog
    mHelpDialog->show();
}

//==============================================================================
// Launch Import Dialog
//==============================================================================
void MainWindow::launchImportDialog()
{
/*
    qDebug() << "MainWindow::launchImportDialog";

*/

}

//==============================================================================
// Launch Export Dialog
//==============================================================================
void MainWindow::launchExportDialog()
{
/*
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::launchExportDialog";

    }
*/
}

//==============================================================================
// Load Plugins
//==============================================================================
void MainWindow::loadPlugins()
{
//    qDebug() << "MainWindow::loadPlugins";

    // Get Plugin Path List
    QStringList pluginPathList = mPreferences->pluginList();

    qDebug() << "MainWindow::loadPlugins - count: " << pluginPathList.count();

    // Iterate Through Plugin List
    for (QString pluginPath : pluginPathList) {
        // Load Plugin
        PluginInfo pluginInfo = PluginInfo::loadPlugin(pluginPath, false);

        qDebug() << "MainWindow::loadPlugins - mName: " << pluginInfo.mName << " (" << pluginInfo.mVersion << ")";

        // Switch Type
        switch (pluginInfo.mType) {
            case 0: {
                // Get Import Plugin Instance
                ITranslationEditorImportPluginInterface* importPluginInstance = static_cast<ITranslationEditorImportPluginInterface*>(pluginInfo.mInstance);
                // Connect Import Ready Signal
                connect(importPluginInstance, &ITranslationEditorImportPluginInterface::importReady, this, &MainWindow::packageImportReady);
                // Append To Import Plugins
                mImportPlugins << pluginInfo;
            } break;

            case 1: {
//                // Get Export Plugin Instance
//                ITranslationEditorExportPluginInterface* exportPluginInstance = dynamic_cast<ITranslationEditorExportPluginInterface*>(pluginInfo.mInstance);

                // Append To Export Plugins
                mExportPlugins << pluginInfo;
            } break;

            default:
                // Check Instance
                if (pluginInfo.mInstance) {
                    // Delete Instance
                    delete pluginInfo.mInstance;
                }
            break;
        }
    }
}

//==============================================================================
// Clear Plugins
//==============================================================================
void MainWindow::clearPlugins()
{
    qDebug() << "MainWindow::clearPlugins";

    // Iterate Through Import Plugins
    for (PluginInfo pluginInfo : mImportPlugins) {
        // Check Instance
        if (pluginInfo.mInstance) {
            // Delete Instance
            delete pluginInfo.mInstance;
        }
    }

    // Iterate Through Export Plugins
    for (PluginInfo pluginInfo : mExportPlugins) {
        // Check Instance
        if (pluginInfo.mInstance) {
            // Delete Instance
            delete pluginInfo.mInstance;
        }
    }

    // Clear Plugins
    mImportPlugins.clear();
    mExportPlugins.clear();
}

//==============================================================================
// Update Menu
//==============================================================================
void MainWindow::updateMenu()
{
    qDebug() << "MainWindow::updateMenu";

    // ...

    ui->actionSave->setEnabled(mCurrentPackage && mCurrentPackage->isDirty());
    ui->actionSaveAs->setEnabled(mCurrentPackage);
    ui->actionClose->setEnabled(!mCheckingActive && mCurrentPackage);
    ui->actionProjectPreferences->setEnabled(!mCheckingActive && mCurrentPackage);

    ui->menuRecents->setEnabled(mRecentFiles.count() > 0);
    ui->actionClearAllRecents->setEnabled(mRecentFiles.count() > 0);

    ui->actionSelectAll->setEnabled(!mCheckingActive && mCurrentPackage);
    ui->actionSelectNone->setEnabled(!mCheckingActive && mCurrentPackage);

    ui->actionShowHidePreview->setEnabled(!mCheckingActive && mCurrentItem);

    ui->actionAddGroup->setEnabled(!mCheckingActive && mCurrentPackage);
    ui->actionRemoveGroup->setEnabled(!mCheckingActive && mCurrentPackage && mCurrentPackage->groups().count() > 0);

    ui->actionNewTranslationItem->setEnabled(!mCheckingActive && mCurrentPackage && mCurrentPackage->groups().count() > 0);
    ui->actionDeleteTranslationItem->setEnabled(!mCheckingActive && mCurrentItem);

    ui->actionFindItems->setEnabled(!mCheckingActive && mCurrentPackage);
    ui->actionFilter->setEnabled(!mCheckingActive && mCurrentPackage);

    ui->actionAddLayout->setEnabled(!mCheckingActive && mCurrentItem);
    ui->actionRemoveLayout->setEnabled(!mCheckingActive && mCurrentItem && mCurrentItem->layoutIndex() > 0);

    ui->actionCheckTranslation->setEnabled(mCurrentItem && !mCheckingActive);
    ui->actionCheckAllTranslations->setEnabled(mCurrentPackage && !mCheckingActive);

    ui->menuImport->setEnabled(!mCheckingActive && mImportPlugins.count() > 0);
    ui->menuExport->setEnabled(!mCheckingActive && mCurrentPackage && (mExportPlugins.count() > 0));

    ui->menuRecents->setEnabled(mRecentFiles.count() > 0 && !mCheckingActive);

#if !defined(Q_OS_WIN)

    // Check Exit Action
    if (ui->menuFile) {
        // Remove Exit
        ui->menuFile->removeAction(ui->actionExit);
    }

#endif

    // ...
}

//==============================================================================
// Update Import/Export Submenu
//==============================================================================
void MainWindow::updateImportExportSubMenu()
{
    qDebug() << "MainWindow::updateImportExportSubMenu";

    // Clear Import Menu
    ui->menuImport->clear();
    // Clear Export Menu
    ui->menuExport->clear();

    // Get Import Plugins Count
    int ipCount = mImportPlugins.count();

    // Iterate Through Import Plugins
    for (int i=0; i<ipCount; i++) {
        // Get PLugin Info
        PluginInfo pluginInfo = mImportPlugins[i];
        // Add Action
        QAction* newImportAction = ui->menuImport->addAction(pluginInfo.mMenuText, this, SLOT(actionImportTriggered()));
        // Set User Data
        newImportAction->setData(i);
    }

    // Get Export Plugins Count
    int epCount = mExportPlugins.count();

    // Iterate Through Export Plugins
    for (int j=0; j<epCount; j++) {
        // Get Plugin Info
        PluginInfo pluginInfo = mExportPlugins[j];
        // Add Action
        QAction* newExportAction = ui->menuExport->addAction(pluginInfo.mMenuText, this, SLOT(actionExportTriggered()));
        // Set User Data
        newExportAction->setData(j);
    }
}

//==============================================================================
// Update Recent Files Subment
//==============================================================================
void MainWindow::updateRecentFilesSubMenu()
{
    qDebug() << "MainWindow::updateImportExportSubMenu";

    // Clear Recent Submenu
    ui->menuRecents->clear();

    // Get Recent Files Count
    int rCount = mRecentFiles.count();

    // Set Menu Enabled State
    ui->menuRecents->setEnabled(rCount > 0 && !mCheckingActive);

    // Iterate Through Recent Files
    for (int i=0; i<rCount; i++) {
        // Add Action
        QAction* newRecentAction = ui->menuRecents->addAction(mRecentFiles[i], this, SLOT(actionRecentFileTriggered()));
        // Set User Data
        newRecentAction->setData(i);
    }

    // Add Action
    //QAction* newRecentAction = ui->menuImport->addAction("-", this, SLOT(actionRecentFileTriggered()));

    // Add Separator
    QAction* separator = ui->menuRecents->addSeparator();
    // Recet User Data
    separator->setData(-1);

    // Add Clear Recents Item
    QAction* clearRecents = ui->menuRecents->addAction(tr("Clear Recent Files"), this, SLOT(actionRecentFileTriggered()));
    // Set Data
    clearRecents->setData(DEFAULT_RECENT_FILES_MAX);
}

//==============================================================================
// Update Window Title
//==============================================================================
void MainWindow::updateWindowTitle()
{
    // Init Window Title
    QString newTitle = "Translation Editor";

    // Check Current Package
    if (mCurrentPackage) {
        // Append Package Name
        newTitle += QString(" - %1").arg(mCurrentPackage->packageName());

        // Check Dirty State
        if (mCurrentPackage->isDirty()) {
            // Append Dirty Marker
            newTitle += QString(" *");
        }
    }
    // Set Window Title
    setWindowTitle(newTitle);
}

//==============================================================================
// Update Translation Language Selector
//==============================================================================
void MainWindow::updateTranslationLanguageSelector()
{
    // Get Last Language - Have To Do It First
    QString lastLanguage = mPreferences->lastLanguage();

    // Clear Language Selector Combo Box
    ui->languageComboBox->clear();

    // Check Current Package
    if (mCurrentPackage) {
        // Iterate Through Languages
        for (QString language : mCurrentPackage->languages()) {
            // Add Language
            ui->languageComboBox->addItem(getLanguageName(language));
        }

        // Get Index
        int llIndex = mCurrentPackage->languages().indexOf(lastLanguage);

        // Check Index
        if (llIndex != -1) {
            // Set Current Index
            ui->languageComboBox->setCurrentIndex(llIndex);
        } else if (ui->languageComboBox->count() > 0) {
            // Set Current Index
            ui->languageComboBox->setCurrentIndex(0);
        }
    }
}

//==============================================================================
// Update Translation Font Selector
//==============================================================================
void MainWindow::updateTranslationFontSelector()
{
    // Last Index
    int lastIndex = ui->fontComboBox->currentIndex();

    // Clear Font Combo Box
    ui->fontComboBox->clear();

    // Check Current Package
    if (mCurrentPackage) {
        // Init Font Dir Info
        QFileInfo fontDirInfo(mCurrentPackage->fontsDir());

        // Check Font Dir Info
        if (fontDirInfo.exists() && fontDirInfo.isDir()) {
            qDebug() << "MainWindow::updateTranslationFontSelector - fontsDir: " << fontDirInfo.absoluteFilePath();
            // Init Font Dir
            QDir fontDir(fontDirInfo.absoluteFilePath());

            // Init Name Filters
            QStringList nameFilters = QString("*.otf, *.ttf").split(",");

            // Get font Info List
            QFileInfoList fontInfoList = fontDir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

            // Iterate Through Font Info List
            for (QFileInfo fileInfo : fontInfoList) {
                // Add Items
                ui->fontComboBox->addItem(fileInfo.fileName());
            }

            // Check Count
            if (ui->fontComboBox->count() > 0) {
                // Set Current Index
                ui->fontComboBox->setCurrentIndex(lastIndex >= 0 ? lastIndex : 0);
            } else {
                // Set Current Index
                ui->fontComboBox->setCurrentIndex(-1);
            }

            // Emit Current Layout Changed to Load Font
            emit currentLayoutChanged(mCurrentLayout);
        }
    }
}

//==============================================================================
// Update Preview Pane
//==============================================================================
void MainWindow::updatePreviewPane()
{
    //qDebug() << "MainWindow::updatePreviewPane";

    // Get Current Layout
    TranslationItemLayout* currentLayout = mCurrentItem ? mCurrentItem->currentLayout() : nullptr;

    ui->languageComboBox->setEnabled(currentLayout && !mCheckingActive);
    ui->maxWidthSpinner->setEnabled(currentLayout && !mCheckingActive);
    ui->maxHeightSpinner->setEnabled(currentLayout && !mCheckingActive);
    ui->maxLengthSpinner->setEnabled(currentLayout && !mCheckingActive);
    ui->maxLinesSpinner->setEnabled(currentLayout && !mCheckingActive);
    ui->textWrapComboBox->setEnabled(currentLayout && !mCheckingActive);
    ui->elideComboBox->setEnabled(currentLayout && !mCheckingActive);
    ui->fontComboBox->setEnabled(currentLayout && !mCheckingActive);
    ui->fontSizeSpinner->setEnabled(currentLayout && !mCheckingActive);
    ui->altFontsEditButton->setEnabled(currentLayout && !mCheckingActive);

    ui->addLayoutButton->setEnabled(mCurrentItem && !mCheckingActive);

    // Check Current Translation Item
    if (mCurrentItem && currentLayout) {

        // Set Preview Pane Widget Values
        ui->previewTitleLabel->setText(QString("Preview - Layout(%1/%2): %3").arg(mCurrentItem->layoutIndex() + 1)
                                                                             .arg(mCurrentItem->layoutCount())
                                                                             .arg(currentLayout->name()));

        ui->maxWidthSpinner->setValue(currentLayout->maxWidth());
        ui->maxHeightSpinner->setValue(currentLayout->maxHeight());
        ui->maxLengthSpinner->setValue(currentLayout->maxLength());
        ui->clearMaxWidthButton->setEnabled(currentLayout->maxWidth() >= 0);
        ui->clearMaxHeightButton->setEnabled(currentLayout->maxHeight() >= 0);
        ui->clearMaxLengthButton->setEnabled(currentLayout->maxLength() >= 0);
        ui->maxLinesSpinner->setValue(currentLayout->maxLines());
        ui->textWrapComboBox->setCurrentIndex(currentLayout->wrapType());
        ui->elideComboBox->setCurrentIndex(currentLayout->elideType());
        ui->layoutShrinkableCheckBox->setChecked(currentLayout->shrinkable());
        ui->alternativeFontsLabel->setText(currentLayout->altFonts().isEmpty() ? "" : currentLayout->altFonts().join(";"));

        // Get Font Combo Box Count
        int fcCount = ui->fontComboBox->count();
        // New Index
        int newIndex = fcCount > 0 ? 0 : -1;
        // Iterate Through Items
        for (int f=0; f<fcCount; f++) {
            // Check Item Text
            if (ui->fontComboBox->itemText(f) == currentLayout->fontName()) {
                // Set New Index
                newIndex = f;
                break;
            }
        }

        ui->fontComboBox->setCurrentIndex(newIndex);
        ui->fontSizeSpinner->setValue(currentLayout->fontSize());
        ui->prevLayoutButton->setEnabled((mCurrentItem->layoutIndex() > 0) && !mCheckingActive);
        ui->nextLayoutButton->setEnabled((mCurrentItem->layoutIndex() < mCurrentItem->layoutCount() - 1) && !mCheckingActive);
        ui->removeLayoutButton->setEnabled((mCurrentItem->layoutIndex() > 0) && !mCheckingActive);

        // ...

    } else {
        // Reset Preview Pane Widget Values
        ui->previewTitleLabel->setText("Preview:");

        ui->maxWidthSpinner->setValue(-1);
        ui->maxHeightSpinner->setValue(-1);
        ui->maxLengthSpinner->setValue(-1);
        ui->maxLinesSpinner->setValue(-1);
        ui->textWrapComboBox->setCurrentIndex(0);
        ui->elideComboBox->setCurrentIndex(3);
        ui->fontComboBox->setCurrentIndex(0);
        ui->fontSizeSpinner->setValue(DEFAULT_TEXT_FONT_SIZE);
        ui->prevLayoutButton->setEnabled(false);
        ui->nextLayoutButton->setEnabled(false);
        ui->removeLayoutButton->setEnabled(false);
        ui->layoutShrinkableCheckBox->setChecked(false);
        ui->alternativeFontsLabel->setText("");
        ui->altFontsEditButton->setEnabled(false);

        // ...
    }
}

//==============================================================================
// Current Package Dirty State Changed Slot
//==============================================================================
void MainWindow::currentPackageDirtyChanged(const bool& /*aDirty*/)
{
    // Set Enabled State
    ui->actionSave->setEnabled(mCurrentPackage && mCurrentPackage->isDirty());

    // Update Window Title
    updateWindowTitle();

    // ...
}

//==============================================================================
// Package Import Ready Signal
//==============================================================================
void MainWindow::packageImportReady(const QJsonObject& aImportedPackage)
{
    qDebug() << "MainWindow::packageImportReady";

    // Reset Busy State
    setBusy(false);

    // Check Imported Package JSON Object
    if (!aImportedPackage.isEmpty()) {
        // Close Current Package First...
        closePackage();
        // Create New Package
        PackageInfo* newPackage = PackageInfo::fromJSON(aImportedPackage);
        // Set Fonts Dir To Default
        newPackage->setFontsDir(mPreferences->defaultFontDir());
        // Set Current Package
        setCurrentPackage(newPackage);

        // ...

    }
}

//==============================================================================
// Package Export Ready Slot
//==============================================================================
void MainWindow::packageExportReady()
{
    qDebug() << "MainWindow::packageExportReady";

    // Reset Busy State
    setBusy(false);

    // ...

}

//==============================================================================
// Package Langauges Changed Slot
//==============================================================================
void MainWindow::packageLanguagesChanged(const QStringList& newLanguages)
{
    qDebug() <<"MainWindow::packageLanguagesChanged - newLanguages: " << newLanguages;

    // Update Available Languages
    updateTranslationLanguageSelector();

    // ...
}

//==============================================================================
// Translation Item About To Be Removed Slot
//==============================================================================
void MainWindow::itemAboutToBeRemoved(const int& aIndex)
{
    qDebug() << "MainWindow::itemAboutToBeRemoved - aIndex: " << aIndex;
    // Set Last Current Index
    mLastIndex = mCurrentIndex;
    // Reset Translation Item
    selectTranslationItem(-1);
}

//==============================================================================
// Translation Item Removed Slot
//==============================================================================
void MainWindow::itemRemoved(const int& aIndex)
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "MainWindow::itemRemoved - aIndex: " << aIndex << " - mLastIndex: " << mLastIndex;
//        // Preserve Current Index
//        int preservedIndex = mCurrentIndex;
//        // Select No Translation Item
//        selectTranslationItem(-1);
        // Select Translation Item
        selectTranslationItem(qBound(-1, mLastIndex, mCurrentPackage->rowCount()-1));
        // Reset Last Current Index
        mLastIndex = -1;
    }
}

////==============================================================================
//// Search Result Item Selected Slot
////==============================================================================
//void MainWindow::searchResultItemSelected(const int& aIndex)
//{
//    qDebug() << "MainWindow::searchResultItemSelected - aIndex: " << aIndex;

//    // Select Translation Item
//    //selectTranslationItem(aIndex);

//    // ...
//}

//==============================================================================
// Action Import Triggered Slot
//==============================================================================
void MainWindow::actionImportTriggered()
{
    // Get Sender Action
    QAction* importAction = dynamic_cast<QAction*>(sender());

    // Check Import Action
    if (importAction) {
        // Get Import Plugin Index
        int ipIndex = importAction->data().toInt();

        qDebug() << "MainWindow::actionImportTriggered - ipIndex: " << ipIndex;

        // Get Plugin Info
        PluginInfo pluginInfo = mImportPlugins[ipIndex];

        // Get Import Plugin Instance
        ITranslationEditorImportPluginInterface* importPluginInstance = static_cast<ITranslationEditorImportPluginInterface*>(pluginInfo.mInstance);

        // Check Instance
        if (importPluginInstance) {
            // Get Import Path
            QString importPath = importPluginInstance->getOpenPath();
            // Check Import Path
            if (!importPath.isEmpty()) {
                // Set Busy State
                setBusy(true);
                // Import To Package
                importPluginInstance->importToPackageAsync(importPath, mCurrentPackage->exportToJSON());
                // Emit Busy Text Changed Signal
                emit busyTextChanged("Importing Package...");
            }
        }
    }
}

//==============================================================================
// Action Export Triggered Slot
//==============================================================================
void MainWindow::actionExportTriggered()
{
    // Get Sender Action
    QAction* exportAction = dynamic_cast<QAction*>(sender());

    // Check Export Action
    if (exportAction) {
        // Get Export Plugin Index
        int epIndex = exportAction->data().toInt();

        qDebug() << "MainWindow::actionExportTriggered - epIndex: " << epIndex;

        // Get Plugin Info
        PluginInfo pluginInfo = mExportPlugins[epIndex];

        // Get Export Plugin Instance
        ITranslationEditorExportPluginInterface* exportPluginInstance = static_cast<ITranslationEditorExportPluginInterface*>(pluginInfo.mInstance);

        // Check Instance
        if (exportPluginInstance && mCurrentPackage) {
            // Set Qt Dir
            exportPluginInstance->setQtDir(mPreferences->qtDir());

            // Set Busy State
            setBusy(true);

            // Export Package
            exportPluginInstance->launchExport(mCurrentPackage->exportToJSON());

            // Reset Busy State
            setBusy(false);
        }
    }
}

//==============================================================================
// Action Recent Triggered Slot
//==============================================================================
void MainWindow::actionRecentFileTriggered()
{
    // Get Sender Action
    QAction* recentAction = dynamic_cast<QAction*>(sender());

    // Check Sender Action
    if (recentAction) {
        // Get Recent File Index
        int rIndex = recentAction->data().toInt();

        // Check Recent File Index
        if (rIndex >= 0 && rIndex < mRecentFiles.count()) {
            // Close Package
            closePackage();
            // Load Package
            loadPackage(mRecentFiles[rIndex]);
        } else if (rIndex == DEFAULT_RECENT_FILES_MAX) {
            // Clear Recent Files
            clearRecentFilesList();
        }

        // Update Recent File Menu
        updateRecentFilesSubMenu();
    }
}

//==============================================================================
// Timer Event
//==============================================================================
void MainWindow::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
//        // Check Timer ID
//        if (aEvent->timerId() == mStatusTimerID) {

//        }
    }
}

//==============================================================================
// Resize Event
//==============================================================================
void MainWindow::resizeEvent(QResizeEvent* aEvent)
{
    QMainWindow::resizeEvent(aEvent);

    // Check Window State
    if (!isMinimized() && !isMaximized() && !isFullScreen()) {
        //qDebug() << "MainWindow::resizeEvent - size: [" << aEvent->size().width() << "x" << aEvent->size().height() << "]";

        // Save Last Width
        mPreferences->setLastWindowWidth(aEvent->size().width());
        // Save Last Height
        mPreferences->setLastWindowHeight(aEvent->size().height());
    }
}

//==============================================================================
// Close Ecent
//==============================================================================
void MainWindow::closeEvent(QCloseEvent* aEvent)
{
    QMainWindow::closeEvent(aEvent);

    // Get Window State
    Qt::WindowStates wState = windowState();

    //qDebug() << "MainWindow::closeEvent - windowState: " << wState;

    // Save Last Window State
    mPreferences->setLastWindowState((int)wState);

    // Check Window State
    if (!isMinimized() && !isMaximized() && !isFullScreen()) {
        // Save Last Width
        mPreferences->setLastWindowWidth(width());
        // Save Last Height
        mPreferences->setLastWindowHeight(height());
    }

    // ...
}

//==============================================================================
// Splitter Moved Slot
//==============================================================================
void MainWindow::on_mainSplitter_splitterMoved(int aPos, int aIndex)
{
    Q_UNUSED(aPos);

    // Check Index
    if (aIndex == 1) {
        //qDebug() << "MainWindow::on_mainSplitter_splitterMoved - aIndex: " << aIndex << " - aPos: " << aPos;
        // Get Preview Pane Width
        int previewWidth = ui->mainSplitter->sizes()[1];

        //qDebug() << "MainWindow::on_mainSplitter_splitterMoved - previewWidth: " << previewWidth;

        // Check Preview Width
        if (previewWidth > 0) {
            // Save Preview Width
            mPreferences->setPreviewWidth(previewWidth);
        }
    }
}

//==============================================================================
// On Action New Package/Project Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_triggered()
{
    // Create New Package
    createNewPackage();
}

//==============================================================================
// Action Open Triggered Slot
//==============================================================================
void MainWindow::on_actionOpen_triggered()
{
    // Load Package
    loadPackage();
}

//==============================================================================
// Action Save Package Triggered Slot
//==============================================================================
void MainWindow::on_actionSave_triggered()
{
    // Save Package
    savePackage();
}

//==============================================================================
// Action Save As Triggered Slot
//==============================================================================
void MainWindow::on_actionSaveAs_triggered()
{
    // Save As Package
    saveAsPackage();
}

//==============================================================================
// Action Close Package Triggered Slot
//==============================================================================
void MainWindow::on_actionClose_triggered()
{
    // Close Package
    closePackage();
}

//==============================================================================
// Action Clear Recent Files Triggered Slot
//==============================================================================
void MainWindow::on_actionClearAllRecents_triggered()
{
    // Clear Recent Files List
    clearRecentFilesList();
}

//==============================================================================
// Action Show Hide Preview Triggered Slot
//==============================================================================
void MainWindow::on_actionShowHidePreview_triggered()
{
    // Show/Hide Preview Pane
    showHidePreview();
}

//==============================================================================
// Action Select All Triggered Slot
//==============================================================================
void MainWindow::on_actionSelectAll_triggered()
{
    // Select All Translations
    selectAllTranslations();
}

//==============================================================================
// Action Select None Triggered Slot
//==============================================================================
void MainWindow::on_actionSelectNone_triggered()
{
    // Select None Translations
    selectNoneTranslations();
}

//==============================================================================
// Action New Translation Item Triggered Slot
//==============================================================================
void MainWindow::on_actionNewTranslationItem_triggered()
{
    // Add Translation Item
    addTranslationItem();
}

//==============================================================================
// Action Delete Translation Item Triggered Slot
//==============================================================================
void MainWindow::on_actionDeleteTranslationItem_triggered()
{
    // Remove Translation Item
    removeTranslationItem();
}

//==============================================================================
// Action Add Translation Group Triggered Slot
//==============================================================================
void MainWindow::on_actionAddGroup_triggered()
{
    // Add Group
    addTranslationGroup();
}

//==============================================================================
// Action Remove Translation Group Triggered Slot
//==============================================================================
void MainWindow::on_actionRemoveGroup_triggered()
{
    // Remove Group
    removeTranslationGroup();
}

//==============================================================================
// Action Add Layout Triggered Slot
//==============================================================================
void MainWindow::on_actionAddLayout_triggered()
{
    // Add Translation Layout
    addTranslationLayout();
}

//==============================================================================
// Action Remove Layout Triggered Slot
//==============================================================================
void MainWindow::on_actionRemoveLayout_triggered()
{
    // Remove Translation Layout
    removeTranslationLayout();
}

//==============================================================================
// Action Find Items Triggered Slot
//==============================================================================
void MainWindow::on_actionFindItems_triggered()
{
    // Launch Find Dialog
    launchFindDialog();
}

//==============================================================================
// Action Filter Triggered Slot
//==============================================================================
void MainWindow::on_actionFilter_triggered()
{
    // Emit Toggle Filter Editor Signal
    emit toggleFilterEditor();
}

//==============================================================================
// Action Project Preferences Slot
//==============================================================================
void MainWindow::on_actionProjectPreferences_triggered()
{
    // Launch Package/Project Preferences Dialog
    launchPackagePreferencesDialog();
}

//==============================================================================
// Action Preferences Triggered Slot
//==============================================================================
void MainWindow::on_actionPreferences_triggered()
{
    // Launch Preferences Dialog
    launchPreferencesDialog();
}

//==============================================================================
// Action Help Triggered Slot
//==============================================================================
void MainWindow::on_actionHelp_triggered()
{
    // Launch Help Dialog
    launchHelpDialog();
}

//==============================================================================
// Action About Triggered Slot
//==============================================================================
void MainWindow::on_actionAbout_triggered()
{
    // Launch About Dialog
    launchAboutDialog();
}

//==============================================================================
// On Close Preview Button Clicked Slot
//==============================================================================
void MainWindow::on_closePreviewButton_clicked()
{
    // Hide Preview Pane
    hidePreviewPane();
}

//==============================================================================
// Add Translation Layout Button Clicked Slot
//==============================================================================
void MainWindow::on_addLayoutButton_clicked()
{
    addTranslationLayout();
}

//==============================================================================
// Previous Translation Layout Button Clicked Slot
//==============================================================================
void MainWindow::on_prevLayoutButton_clicked()
{
    // Goto Previous Translation Layout
    prevTranslationLayout();
}

//==============================================================================
// Next Translation Layout Button clicked Slot
//==============================================================================
void MainWindow::on_nextLayoutButton_clicked()
{
    // Goto Next Translation Layout
    nextTranslationLayout();
}

//==============================================================================
// Remove Translation Layout Button Clicked Slot
//==============================================================================
void MainWindow::on_removeLayoutButton_clicked()
{
    // Remove Current Translation Item Layout
    removeTranslationLayout();
}

//==============================================================================
// Language Combo Box Current Index Changed Slot
//==============================================================================
void MainWindow::on_languageComboBox_currentIndexChanged(int index)
{
    // Set Selected Translation Language
    setSelectedTranslationLanguage(index);
}

//==============================================================================
// Max Width Spinner Value Changed Slot
//==============================================================================
void MainWindow::on_maxWidthSpinner_valueChanged(int arg1)
{
    // Set Current Layout Max Width
    setLayoutMaxWidth(arg1);
}

//==============================================================================
// Max Height Spinner Value Changed Slot
//==============================================================================
void MainWindow::on_maxHeightSpinner_valueChanged(int arg1)
{
    // Set Current Layout Max Height
    setLayoutMaxHeight(arg1);
}

//==============================================================================
// Max Length Spinner Value Changed Slot
//==============================================================================
void MainWindow::on_maxLengthSpinner_valueChanged(int arg1)
{
    // Set Current Layout Max Length
    setLayoutMaxLength(arg1);
}

//==============================================================================
// Clear Max Width Button Clicked Slot
//==============================================================================
void MainWindow::on_clearMaxWidthButton_clicked()
{
    // Reset Current Layout Max Width
    setLayoutMaxWidth(-1);
}

//==============================================================================
// Clear Max Height Button Clicked Slot
//==============================================================================
void MainWindow::on_clearMaxHeightButton_clicked()
{
    // Reset Current Layout Max Height
    setLayoutMaxHeight(-1);
}

//==============================================================================
// Clear Max Length Button Clicked Slot
//==============================================================================
void MainWindow::on_clearMaxLengthButton_clicked()
{
    // Reset Layout Max Length
    setLayoutMaxLength(-1);
}

//==============================================================================
// Max Lines Spinner Value Changed Slot
//==============================================================================
void MainWindow::on_maxLinesSpinner_valueChanged(int arg1)
{
    // Set Current Layout Max Lines
    setLayoutMaxLines(arg1);
}

//==============================================================================
// Wrap Type Combo Box Current Index Changed Slot
//==============================================================================
void MainWindow::on_textWrapComboBox_currentIndexChanged(int index)
{
    // Set Current Layout Wrap Type
    setLayoutWrapType(index);
}

//==============================================================================
// Elide Type Combo Box Current Index Changed Slot
//==============================================================================
void MainWindow::on_elideComboBox_currentIndexChanged(int index)
{
    // Set Current Layout Elide Type
    setLayoutElideType(index);
}

//==============================================================================
// Translation Font Combo Box Current Index Changed Slot
//==============================================================================
void MainWindow::on_fontComboBox_currentIndexChanged(int index)
{
    // Set Layout Font Name By Index
    setLayoutFontName(index);
}

//==============================================================================
// Translation Font Size Spinner Value Changed Slot
//==============================================================================
void MainWindow::on_fontSizeSpinner_valueChanged(int arg1)
{
    // Set Current Layout Font Size
    setLayoutFontSize(arg1);
}

//==============================================================================
// Layout Shrinkable Checkbox Clicked Slot
//==============================================================================
void MainWindow::on_layoutShrinkableCheckBox_clicked()
{
    // Set Layout Shrinkable
    setLayoutShrinkable(ui->layoutShrinkableCheckBox->checkState() == Qt::Checked);
}

//==============================================================================
// Alternative Fonts Edit Button Clicked Slot
//==============================================================================
void MainWindow::on_altFontsEditButton_clicked()
{
    // Launch Alternative Fonts Edit Dialog
    launchAlternativeFontsDialog();
}

//==============================================================================
// Action Check Translation Triggered Slot
//==============================================================================
void MainWindow::on_actionCheckTranslation_triggered()
{
    // Check Current Translation Item
    checkTranslationItem();
}

//==============================================================================
// Action Check All Translations Triggered Slot
//==============================================================================
void MainWindow::on_actionCheckAllTranslations_triggered()
{
    // Check All Translations
    checkAllTranslations();
}

//==============================================================================
// Action Quit Triggered Slot
//==============================================================================
void MainWindow::on_actionQuit_triggered()
{
    // Close Package
    closePackage();

    // Quit
    qApp->quit();
}

//==============================================================================
// Action Exit Triggered
//==============================================================================
void MainWindow::on_actionExit_triggered()
{
    // Just Call Quit
    on_actionQuit_triggered();
}

//==============================================================================
// Destructor
//==============================================================================
MainWindow::~MainWindow()
{
    // Shut Down
    shutDown();

    if (mPackagePreferencesDialog) {
        delete mPackagePreferencesDialog;
    }

    // Delete Quick Widgets First
    delete ui->mainQuickWidget;
    delete ui->previewQuickWidget;

    delete ui;

    qDebug() << "MainWindow deleted.";
}

