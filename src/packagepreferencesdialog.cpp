#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>
#include <QFileDialog>

#include "src/packagepreferencesdialog.h"
#include "ui_packagepreferencesdialog.h"

#include "packageinfo.h"
#include "translationitemlayout.h"
#include "shared/translation_keys.h"
#include "mainwindow.h"
#include "preferences.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
PackagePreferencesDialog::PackagePreferencesDialog(MainWindow* aMainWidnow, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PackagePreferencesDialog)
    , mMainWindow(aMainWidnow)
    , mPreferences(Preferences::getInstance())
    , mCurrentPackage(nullptr)
{
    qDebug() << "PackagePreferencesDialog created.";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void PackagePreferencesDialog::init()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Get QML Engine
    QQmlEngine* engine = ui->languagesQuickWidget->engine();
    // Get QML Context
    QQmlContext* qmlContext = engine ? engine->rootContext() : nullptr;
    // Check QML Context
    if (qmlContext) {
        // Set Context Property - Package Controller
        qmlContext->setContextProperty(CONTEXT_PROPERTY_MAIN_CONTROLLER, mMainWindow);
        // Set Context Property - Package Controller
        qmlContext->setContextProperty(CONTEXT_PROPERTY_PACKAGE_CONTROLLER, this);
        // Set Context Property - Preferences
        qmlContext->setContextProperty(CONTEXT_PROPERTY_PREFERENCES, mPreferences);

        // ...
    }

    // Set Source
    ui->languagesQuickWidget->setSource(QUrl(PACKAGE_PREFERENCES_QML_SOURCE));
}

//==============================================================================
// Get Current Package
//==============================================================================
PackageInfo* PackagePreferencesDialog::currentPackage()
{
    return mCurrentPackage;
}

//==============================================================================
// Load Package Preferences
//==============================================================================
void PackagePreferencesDialog::loadPreferences(PackageInfo* aPackageInfo)
{
    // Check Current Package
    if (mCurrentPackage != aPackageInfo) {
        // Set Current Package
        mCurrentPackage = aPackageInfo;
        // Emit Current Package Changed Signal
        emit currentPackageChanged(mCurrentPackage);
    }

    // Check Package Info
    if (mCurrentPackage) {
        // Set Package Name
        ui->packageNameEdit->setText(mCurrentPackage->packageName());
        // Set Fonts Dir
        ui->fontDirEdit->setText(mCurrentPackage->fontsDir());

        // Set Default Font  TODO: Update!!
        ui->defaultFontComboBox->setCurrentText(mCurrentPackage->defaultFont());

        // Set Default Font Size
        ui->defaultFontSizeSpinner->setValue(mCurrentPackage->defaultFontSize());

        // Supported Languages

        // ...
    }
}

//==============================================================================
// Save Preferences
//==============================================================================
void PackagePreferencesDialog::savePreferences()
{
    // Check Current Package
    if (mCurrentPackage) {
        qDebug() << "PackagePreferencesDialog::savePreferences";

        // Set Name
        mCurrentPackage->setPackageName(ui->packageNameEdit->text());
        // Set Fonts Dir
        mCurrentPackage->setFontsDir(ui->fontDirEdit->text());
        // Set Default Font
        mCurrentPackage->setDefaultFont(ui->defaultFontComboBox->currentText());
        // Set Default Font Size
        mCurrentPackage->setDefaultFontSize(ui->defaultFontSizeSpinner->value());

        // Supported Languages

        // ...
    }
}

//==============================================================================
// Load Defaults
//==============================================================================
void PackagePreferencesDialog::loadDefaults()
{
    qDebug() << "PackagePreferencesDialog::loadDefaults";

    // Set Name Edit
    ui->packageNameEdit->setText("New Package");
    // Select All
    ui->packageNameEdit->selectAll();
    // Set Fonts Dir
    ui->fontDirEdit->setText(mPreferences->defaultFontDir());

//    // Clear
//    ui->defaultFontComboBox->clear();
//    // Load Fonts

    // Set Current Index
    ui->defaultFontComboBox->setCurrentIndex(0);
    // Set Default Font
    ui->defaultFontComboBox->setCurrentText("");

    // Set Default Font Size
    ui->defaultFontSizeSpinner->setValue(DEFAULT_TEXT_FONT_SIZE);
}

//==============================================================================
// ShutDown
//==============================================================================
void PackagePreferencesDialog::shutDown()
{
    // Check Preferences
    if (mPreferences) {
        // Release Instance
        mPreferences->release();
        // Reset Preferences Pointer
        mPreferences = nullptr;
    }
}

//==============================================================================
// Build Font List
//==============================================================================
void PackagePreferencesDialog::buildFontList(const QString& aPath)
{
    // Clear Font List
    ui->defaultFontComboBox->clear();

    // Init Font Dir Info
    QFileInfo fontDirInfo(aPath);

    // Check Font Dir Info
    if (fontDirInfo.exists() && fontDirInfo.isDir()) {
        qDebug() << "PackagePreferencesDialog::buildFontList - aPath: " << aPath;
        // Init Font Dir
        QDir fontDir(aPath);

        // Init Name Filters
        QStringList nameFilters = QString("*.otf, *.ttf").split(",");

        // Get font Info List
        QFileInfoList fontInfoList = fontDir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

        // Iterate Through Font Info List
        for (QFileInfo fileInfo : fontInfoList) {
            // Add Items
            ui->defaultFontComboBox->addItem(fileInfo.fileName());
        }

        // Check Count
        if (ui->defaultFontComboBox->count() > 0) {
            // Set Current Index
            ui->defaultFontComboBox->setCurrentIndex(0);
        } else {
            // Set Current Index
            ui->defaultFontComboBox->setCurrentIndex(-1);
        }
    }
}

//==============================================================================
// Set Current Package
//==============================================================================
void PackagePreferencesDialog::setCurrentPackage(PackageInfo* aPackageInfo)
{
    // Check Current Package
    if (mCurrentPackage != aPackageInfo) {
        // Set Current Package
        mCurrentPackage = aPackageInfo;

        // ...

        // Emit Current Pacakge Changed Signal
        emit currentPackageChanged(mCurrentPackage);
    }
}

//==============================================================================
// Discard New Package
//==============================================================================
void PackagePreferencesDialog::discardNewPackage()
{
    // Check Current Package
    if (mCurrentPackage) {
        // Delete Current Package
        delete mCurrentPackage;
    }

    // Reset Current Package
    setCurrentPackage(nullptr);
}

//==============================================================================
// On Font Dir Edit Text Changed Slot
//==============================================================================
void PackagePreferencesDialog::on_fontDirEdit_textChanged(const QString& newText)
{
    //qDebug() << "PackagePreferencesDialog::on_fontDirEdit_textChanged - newText: " << newText;

    // Build Font List
    buildFontList(newText);
}

//==============================================================================
// Browse Font Dir Button Clicked Slot
//==============================================================================
void PackagePreferencesDialog::on_browseFontDirButton_clicked()
{
    // Init Initial Path
    QString initialPath = ui->fontDirEdit->text().isEmpty() ? QDir::homePath() : ui->fontDirEdit->text();
    // Get Existing Directory
    QString newFontsDir = QFileDialog::getExistingDirectory(nullptr, "Select Font Dir", initialPath);
    // Check New Fonts Dir
    if (!newFontsDir.isEmpty()) {
        // Set Text
        ui->fontDirEdit->setText(newFontsDir);
    }
}

//==============================================================================
// Destructor
//==============================================================================
PackagePreferencesDialog::~PackagePreferencesDialog()
{
    // Shut Down
    shutDown();

    // Delete Languages Widget First
    delete ui->languagesQuickWidget;
    // Delete UI
    delete ui;

    qDebug() << "PackagePreferencesDialog deleted.";
}

