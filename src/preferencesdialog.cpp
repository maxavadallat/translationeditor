#include <QDebug>
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QRgb>
#include <QPushButton>
#include <QJsonObject>
#include <QPluginLoader>
#include <QFileInfo>
#include <QFileDialog>
#include <QQmlEngine>
#include <QQmlContext>

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "preferences.h"
#include "plugininfo.h"
#include "shared/translationeditorexportplugininterface.h"
#include "shared/translationeditorimportplugininterface.h"
#include "constants.h"
#include "settings_defaults.h"

// GUI Font select Buton Font Size
#define GUI_FONT_SELECT_BUTTON_FONT_SIZE                    11

// Default Sizes
#define PREFERENCES_DIALOG_WIDTH_DEFAULT                    480
#define PREFERENCES_DIALOG_WIDTH_COLORS                     980

// Context Property Names
#define COLORS_PREVIEW_CONTEXT_PROPERTY_FONT_SIZE           "cpFontSize"
#define COLORS_PREVIEW_CONTEXT_PROPERTY_ROW_SIZE            "cpRowSize"

#define COLORS_PREVIEW_CONTEXT_PROPERTY_ALT_ROWS            "cpAltRows"

#define COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_TEXT          "cpColorText"
#define COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BACKGROUND    "cpColorBackground"
#define COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BORDER        "cpColorBorder"
#define COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_ROWALT        "cpColorRowAlt"
#define COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HEADER        "cpColorHeader"
#define COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HIGHLIGHT     "cpColorHighlight"


//==============================================================================
// RGB To String
//==============================================================================
QString rgb2string(const QRgb& aColor)
{
    // Convert To Result
    QString result = QString("#%1%2%3%4").arg(qAlpha(aColor), 2, 16, QChar('0'))
                                         .arg(qRed(aColor), 2, 16, QChar('0'))
                                         .arg(qGreen(aColor), 2, 16, QChar('0'))
                                         .arg(qBlue(aColor), 2, 16, QChar('0'));

    return result;
}

//==============================================================================
// String To RGB
//==============================================================================
QRgb string2Rgb(const QString& aColor)
{
    // Convert To Result
    QRgb result = qRgba(aColor.mid(3, 2).toInt(nullptr, 16),
                        aColor.mid(5, 2).toInt(nullptr, 16),
                        aColor.mid(7, 2).toInt(nullptr, 16),
                        aColor.mid(1, 2).toInt(nullptr, 16));

    return result;
}


//==============================================================================
// Constructor
//==============================================================================
PreferencesDialog::PreferencesDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PreferencesDialog)
    , mPreferences(Preferences::getInstance())
    , mDirty(false)
    , mLastPluginDir("")
    , mCustomColorsView(false)
    , mColorsQMLContext(nullptr)
{
    qDebug() << "PreferencesDialog created.";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void PreferencesDialog::init()
{
    qDebug() << "PreferencesDialog::init";

    // Connect Button Box Signal
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
    // Connect Preferences Dirty Changed Signal
    connect(mPreferences, SIGNAL(dirtyChanged(bool)), this, SLOT(dirtyStateChanged(bool)));

    // Set Enabled State - Apply
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(mDirty);

    // Get Color Preview Quick Widget Engine
    QQmlEngine* mainEngine = ui->colorPreviewQuickWidget->engine();
    // Get Color Preview Quick Widget Context
    mColorsQMLContext = mainEngine ? mainEngine->rootContext() : nullptr;

    // Check Colors Preview QML Context
    if (mColorsQMLContext) {
        // Set Context Property - Preferences
        mColorsQMLContext->setContextProperty(CONTEXT_PROPERTY_PREFERENCES, mPreferences);

        // Init Context Properties
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_FONT_SIZE, SETTINGS_DEFAULT_GUI_FONTSIZE);
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_ROW_SIZE, SETTINGS_DEFAULT_GUI_ROWHEIGHT);

        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_ALT_ROWS, true);

        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_TEXT, SETTINGS_DEFAULT_COLOR_TEXT);
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BACKGROUND, SETTINGS_DEFAULT_COLOR_BACKGROUND);
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BORDER, SETTINGS_DEFAULT_COLOR_BORDER);
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_ROWALT, SETTINGS_DEFAULT_COLOR_ROW_ALT);
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HEADER, SETTINGS_DEFAULT_COLOR_HEADER);
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HIGHLIGHT, SETTINGS_DEFAULT_COLOR_HIGHLIGHT);
    }

    // Set Main Quick Widget Source
    ui->colorPreviewQuickWidget->setSource(QUrl(COLOR_PREVIEW_SOURCE));

    //qDebug() << "#### error: " << ui->colorPreviewQuickWidget->status();

    // Load Preferences
    loadPreferences();
}

//==============================================================================
// Clear
//==============================================================================
void PreferencesDialog::clear()
{
    qDebug() << "PreferencesDialog::clear";

    // Clear Plugin List
    mPluginsList.clear();
}

//==============================================================================
// Shut Down
//==============================================================================
void PreferencesDialog::shutDown()
{
    qDebug() << "PreferencesDialog::shutDown";

    // Clear
    clear();
}

//==============================================================================
// Set Dirty State
//==============================================================================
void PreferencesDialog::setDirty(const bool& aDirty)
{
    // Check Dirty State
    if (mDirty != aDirty) {
        qDebug() << "PreferencesDialog::setDirty - aDirty: " << aDirty;
        // Set Dirty State
        mDirty = aDirty;

        // ...

        // Update Buttons
        updateButtons();
    }
}

//==============================================================================
// Load PLugins
//==============================================================================
void PreferencesDialog::loadPluginsList()
{
    qDebug() << "PreferencesDialog::loadPluginsList";

    // Get Plugin List
    QStringList pluginList = mPreferences->pluginList();

    // Iterate Through Plugins List
    for (QString pluginPath : pluginList) {
        // Add Plugin Info
        addPluginInfo(loadPlugin(pluginPath), false);
    }
}

//==============================================================================
// Update Buttons
//==============================================================================
void PreferencesDialog::updateButtons()
{
    qDebug() << "PreferencesDialog::updateButtons";

    // Set Enabled State - Apply
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(mDirty);

    // ...
}

//==============================================================================
// Update PLugin List
//==============================================================================
void PreferencesDialog::updatePluginList()
{
    qDebug() << "PreferencesDialog::updatePluginList";

    // Clear List View
    ui->pluginListView->clear();

    // Iterate Through Plugin List
    for (int i=0; i<mPluginsList.count(); i++) {
        // Add New Item
        ui->pluginListView->addItem(mPluginsList[i].mName + " (" + mPluginsList[i].mVersion + ")");
    }
}

//==============================================================================
// Add Plugin Info
//==============================================================================
void PreferencesDialog::addPluginInfo(const PluginInfo& aPluginInfo, const bool& aUpdateList)
{
    // Check Plugin Type
    if (aPluginInfo.mType != -1) {
        qDebug() << "PreferencesDialog::addPluginInfo - name: " << aPluginInfo.mName << " - aUpdateList: " << aUpdateList;

        // Append PLugin Info
        mPluginsList << aPluginInfo;

        // Check Update List
        if (aUpdateList) {
            // Update Plugin List
            updatePluginList();
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Remove Plugin Info
//==============================================================================
void PreferencesDialog::removePluginInfo(const int& aIndex, const bool& aUpdateList)
{
    // Check Index
    if (aIndex >= 0 && aIndex < mPluginsList.count()) {
        // Remove Plugin Info
        mPluginsList.removeAt(aIndex);
        // Check Update List
        if (aUpdateList) {
            // Update Plugin List
            updatePluginList();
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Load Plugin
//==============================================================================
PluginInfo PreferencesDialog::loadPlugin(const QString& aPath)
{
    return PluginInfo::loadPlugin(aPath);
}

//==============================================================================
// Load Preferences
//==============================================================================
void PreferencesDialog::loadPreferences()
{
    // Clear First
    clear();

    qDebug() << "PreferencesDialog::loadPreferences";

    // Set GUI Theme
    ui->themeComboBox->setCurrentIndex(mPreferences->guiTheme());
    // Init Updaed Font
    QFont currentFont(mPreferences->guiFontName(), GUI_FONT_SELECT_BUTTON_FONT_SIZE);
    // Set GUI Font Name
    ui->fontSelectButton->setText(mPreferences->guiFontName());
    // Set Font
    ui->fontSelectButton->setFont(currentFont);
    // Set GUI Font Size
    ui->fontSizeSpinner->setValue(mPreferences->guiFontSize());
    // Set Row Size
    ui->rowHeightSpinner->setValue(mPreferences->listRowHeight());
    // Set Alternate Row Colors
    ui->alternateRowColorsCheckBox->setChecked(mPreferences->alternateRowColors());
    // Set Default Font Dir
    ui->fontDirEditTextBox->setText(mPreferences->defaultFontDir());
    // Set Qt Dir
    ui->qtDirEditTextBox->setText(mPreferences->qtDir());

    // Set Colors
    ui->colorPickerButtonText->setColor(string2Rgb(mPreferences->colorText()));
    ui->colorPickerButtonBackground->setColor(string2Rgb(mPreferences->colorBackground()));
    ui->colorPickerButtonBorder->setColor(string2Rgb(mPreferences->colorBorder()));
    ui->colorPickerButtonAltRow->setColor(string2Rgb(mPreferences->colorRowAlt()) | 0xFF000000);
    ui->colorPickerButtonHeader->setColor(string2Rgb(mPreferences->colorHeader()));
    ui->colorPickerButtonHighlight->setColor(string2Rgb(mPreferences->colorHighlight()) | 0xFF000000);

    // Check Colors Preview Context
    if (mColorsQMLContext) {
        // Set Context Properties
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_FONT_SIZE, mPreferences->guiFontSize());
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_ROW_SIZE, mPreferences->listRowHeight());

        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_ALT_ROWS, mPreferences->alternateRowColors());

        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_TEXT, mPreferences->colorText());
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BACKGROUND, mPreferences->colorBackground());
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BORDER, mPreferences->colorBorder());
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_ROWALT, mPreferences->colorRowAlt());
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HEADER, mPreferences->colorHeader());
        mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HIGHLIGHT, mPreferences->colorHighlight());
    }

    // Set Custom Colors View
    mCustomColorsView = (ui->themeComboBox->currentIndex() == 2);
    // Resize Dialog
    resize(mCustomColorsView ? PREFERENCES_DIALOG_WIDTH_COLORS : PREFERENCES_DIALOG_WIDTH_DEFAULT, height());

    // ...

    // Load Plugins List
    loadPluginsList();

    // ...

    // Update Buttons
    updateButtons();
    // Update Plugin List
    updatePluginList();

    // Reset Dirty State
    setDirty(false);
}

//==============================================================================
// Load Defults
//==============================================================================
void PreferencesDialog::restoreDefaults()
{
    qDebug() << "PreferencesDialog::restoreDefaults";

    // Load Defaults
    mPreferences->loadDefaults();

    // Load Preferences
    loadPreferences();
}

//==============================================================================
// Apply Settings
//==============================================================================
void PreferencesDialog::applyPreferences()
{
    qDebug() << "PreferencesDialog::applyPreferences";

    // Save Preferences
    savePreferences();
}

//==============================================================================
// Save Preferences
//==============================================================================
void PreferencesDialog::savePreferences()
{
    // Check Dirty State
    if (mDirty) {
        qDebug() << "PreferencesDialog::savePreferences";

        // Set GUI Theme
        mPreferences->setGuiTheme(ui->themeComboBox->currentIndex());
        // Set GUI Font Name
        mPreferences->setGuiFontName(ui->fontSelectButton->text());
        // Set GUI Font Size
        mPreferences->setGuiFontSize(ui->fontSizeSpinner->value());
        // Set List Row Size
        mPreferences->setListRowHeight(ui->rowHeightSpinner->value());
        // Set Alternate Row Colors
        mPreferences->setAlternateRowColors(ui->alternateRowColorsCheckBox->isChecked());
        // Set Default Font Dir
        mPreferences->setDefaultFontDir(ui->fontDirEditTextBox->text());
        // Set Qt Dir
        mPreferences->setQtDir(ui->qtDirEditTextBox->text());

        // Set Colors
        mPreferences->setColorText(rgb2string(ui->colorPickerButtonText->getColor()));
        mPreferences->setColorBackground(rgb2string(ui->colorPickerButtonBackground->getColor()));
        mPreferences->setColorBorder(rgb2string(ui->colorPickerButtonBorder->getColor()));

        // Get Button Color
        QRgb rowAltColor = ui->colorPickerButtonAltRow->getColor();

        // Init New Row Alternative Color
        QString newRowAltColor = QString("#%1%2%3%4").arg(SETTINGS_DEFAULT_ROWALT_ALPHA, 2, 16, QChar('0'))
                                                     .arg(qRed(rowAltColor), 2, 16, QChar('0'))
                                                     .arg(qGreen(rowAltColor), 2, 16, QChar('0'))
                                                     .arg(qBlue(rowAltColor), 2, 16, QChar('0'));
        // Set Color
        mPreferences->setColorRowAlt(newRowAltColor);
        // Set Color
        mPreferences->setColorHeader(rgb2string(ui->colorPickerButtonHeader->getColor()));

        // Get Button Color
        QRgb highlightColor = ui->colorPickerButtonHighlight->getColor();

        // Init New Highlight Color
        QString newHighlightColor = QString("#%1%2%3%4").arg(SETTINGS_DEFAULT_HIGHLIGHT_ALPHA, 2, 16, QChar('0'))
                                                        .arg(qRed(highlightColor), 2, 16, QChar('0'))
                                                        .arg(qGreen(highlightColor), 2, 16, QChar('0'))
                                                        .arg(qBlue(highlightColor), 2, 16, QChar('0'));

        // Set Highlight Color
        mPreferences->setColorHighlight(newHighlightColor);

        // ...

        // Clear Plugins List
        mPreferences->clearPluginList();
        // Iterate Through Plugins
        for (int i=0; i<mPluginsList.count(); i++) {
            // Set Plugins List
            mPreferences->addPlugin(mPluginsList[i].mPath);
        }

        // Save Settings
        mPreferences->saveSettings();

        // ...

        // Reset Dirty State
        setDirty(false);
    }
}

//==============================================================================
// Button Clicked Slot
//==============================================================================
void PreferencesDialog::buttonClicked(QAbstractButton* aButton)
{
    // Get Role
    QDialogButtonBox::ButtonRole buttonRole = ui->buttonBox->buttonRole(aButton);

    qDebug() << "PreferencesDialog::buttonClicked - buttonRole: " << buttonRole;

    // Switch Role
    switch (buttonRole) {
        case QDialogButtonBox::AcceptRole:
        case QDialogButtonBox::ApplyRole: applyPreferences(); return;
        case QDialogButtonBox::ResetRole: restoreDefaults(); return;
        case QDialogButtonBox::RejectRole: clear(); return;
        default:
        break;
    }
}

//==============================================================================
// Dirty State Changed Slot
//==============================================================================
void PreferencesDialog::dirtyStateChanged(const bool& aDirty)
{
    qDebug() << "PreferencesDialog::dirtyStateChanged - aDirty: " << aDirty;

    // Set Enabled State - Apply
    //ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(aDirty);

    // ...
}

//==============================================================================
// On Font Select Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_fontSelectButton_clicked()
{
    // Init Font Dialog
    QFontDialog fontDialog;

    // Exec Font Dialog
    if (fontDialog.exec()) {
        // Get Current Font
        QFont currentFont = fontDialog.currentFont();
        // Set Size
        currentFont.setPixelSize(GUI_FONT_SELECT_BUTTON_FONT_SIZE);
        // Set Button Font
        ui->fontSelectButton->setFont(currentFont);
        // Set Button Text
        ui->fontSelectButton->setText(currentFont.family());

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// GUI Theme Combo Box Current Index Changed Slot
//==============================================================================
void PreferencesDialog::on_themeComboBox_currentIndexChanged(int index)
{
    // Check Visibility
    if (isVisible()) {
        qDebug() << "PreferencesDialog::on_themeComboBox_currentIndexChanged - index: " << index;

        // Set Custom Colors Button Enabled State
        ui->customColorsButton->setEnabled(index == 2);

        // Set Custom Colors View
        mCustomColorsView = (index == 2);
        // Resize Dialog
        resize(mCustomColorsView ? PREFERENCES_DIALOG_WIDTH_COLORS : PREFERENCES_DIALOG_WIDTH_DEFAULT, height());

//        // Check Index
//        if (index == 2) {
//            // Set Custom Colors View
//            mCustomColorsView = true;
//            // Resize Dialog
//            resize(PREFERENCES_DIALOG_WIDTH_COLORS, height());
//        } else {
//            // Reset Custom Colors View
//            mCustomColorsView = false;
//            // Resize Dialog
//            resize(PREFERENCES_DIALOG_WIDTH_DEFAULT, height());
//        }

        // Set Dirty State
        setDirty();

        // ...
    }
}

//==============================================================================
// On Custom Colors Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_customColorsButton_clicked()
{
    // Set Custom Colors View
    mCustomColorsView = !mCustomColorsView;
    // Resize Dialog
    resize(mCustomColorsView ? PREFERENCES_DIALOG_WIDTH_COLORS : PREFERENCES_DIALOG_WIDTH_DEFAULT, height());
}

//==============================================================================
// Font Dir Select Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_fontDirSelectButton_clicked()
{
    // Get Dir Path
    QString newDirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), mPreferences->defaultFontDir());

    // Check New Dir Path
    if (!newDirPath.isEmpty()) {
        // Set Default Font Dir Text Box Text
        ui->fontDirEditTextBox->setText(newDirPath);
    }
}

//==============================================================================
// Font Dir Edit Text Changed Slot
//==============================================================================
void PreferencesDialog::on_fontDirEditTextBox_textChanged(const QString&)
{
    // Check Visibility
    if (isVisible()) {
        qDebug() << "PreferencesDialog::on_fontDirEditTextBox_textChanged";

        // Set Dirty State
        setDirty();

        // ...
    }
}

//==============================================================================
// Qt Dir Select Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_qtDirSelectButton_clicked()
{
    // Get Dir Path
    QString newDirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), mPreferences->qtDir());

    // Check New Dir Path
    if (!newDirPath.isEmpty()) {
        // Set Qt Dir Text Box Text
        ui->qtDirEditTextBox->setText(newDirPath);
    }

}

//==============================================================================
// Qt Dir Edit Text Box Text Changed Slot
//==============================================================================
void PreferencesDialog::on_qtDirEditTextBox_textChanged(const QString&)
{
    // Check Visibility
    if (isVisible()) {
        qDebug() << "PreferencesDialog::on_qtDirEditTextBox_textChanged";

        // Set Dirty State
        setDirty();

        // ...
    }
}

//==============================================================================
// Add Plugin Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_addPluginButton_clicked()
{
#if defined(Q_OS_WIN)
    // Init Filter
    QString filter = "Plugins (*.dll)";
#elif defined(Q_OS_MAC)
    // Init Filter
    QString filter = "Plugins (*.dylib)";
#else // Linux/Unix
    // Init Filter
    QString filter = "Plugins (*.so)";
#endif

    // Check Last Plugin Dir
    if (mLastPluginDir.isEmpty()) {
        // Set Last Plugin Dir
        mLastPluginDir = QDir::homePath();
    }

    // Get Plugin File Path
    QString pluginFilePath = QFileDialog::getOpenFileName(nullptr, "Add Plug-in", mLastPluginDir, filter);

    // Check Plugin Path
    if (!pluginFilePath.isEmpty()) {
        // Update Last Plugin Dir
        mLastPluginDir = QFileInfo(pluginFilePath).absolutePath();
    }

    // Load PLugin Info
    PluginInfo pluginInfo = loadPlugin(pluginFilePath);
    // Add Plugin Info
    addPluginInfo(pluginInfo);
}

//==============================================================================
// Remove Plugin Button Clicked Slot
//==============================================================================
void PreferencesDialog::on_removePluginButton_clicked()
{
    // TODO: Confirmation

    // Remove Plugin Info
    removePluginInfo(ui->pluginListView->currentRow());
}

//==============================================================================
// On Color Picker Button Text Clicked Slot
//==============================================================================
void PreferencesDialog::on_colorPickerButtonText_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();

        // Set Color Picker Button Color
        ui->colorPickerButtonText->setColor(currentColor);

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_TEXT, rgb2string(currentColor));
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// On Color Picker Button Background Clicked Slot
//==============================================================================
void PreferencesDialog::on_colorPickerButtonBackground_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();

        // Set Color Picker Button Color
        ui->colorPickerButtonBackground->setColor(currentColor);

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BACKGROUND, rgb2string(currentColor));
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// On Color Picker Button Border Clicked Slot
//==============================================================================
void PreferencesDialog::on_colorPickerButtonBorder_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();

        // Set Color Picker Button Color
        ui->colorPickerButtonBorder->setColor(currentColor);

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_BORDER, rgb2string(currentColor));
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// On Color Picker Button Alternative Row Clicked Slot
//==============================================================================
void PreferencesDialog::on_colorPickerButtonAltRow_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();

        // Set Color Picker Button Color
        ui->colorPickerButtonAltRow->setColor(currentColor);

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Init New Alternative Row Color
            QString newRowAltColor = QString("#%1%2%3%4").arg(SETTINGS_DEFAULT_ROWALT_ALPHA, 2, 16, QChar('0'))
                                                         .arg(qRed(currentColor), 2, 16, QChar('0'))
                                                         .arg(qGreen(currentColor), 2, 16, QChar('0'))
                                                         .arg(qBlue(currentColor), 2, 16, QChar('0'));

            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_ROWALT, newRowAltColor);
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// On Color Picker Button Header Clicked Slot
//==============================================================================
void PreferencesDialog::on_colorPickerButtonHeader_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();

        // Set Color Picker Button Color
        ui->colorPickerButtonHeader->setColor(currentColor);

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HEADER, rgb2string(currentColor));
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// On Color Picker Button Highlight Clicked Slot
//==============================================================================
void PreferencesDialog::on_colorPickerButtonHighlight_clicked()
{
    // Init Color Dialog
    QColorDialog colorDialog;
    // Exec Color Dialog
    if (colorDialog.exec()) {
        // Get Current Color
        QRgb currentColor = colorDialog.currentColor().rgba();

        // Set Color Picker Button Color
        ui->colorPickerButtonHighlight->setColor(currentColor);

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Init New Highlight Color
            QString newHighlightColor = QString("#%1%2%3%4").arg(SETTINGS_DEFAULT_HIGHLIGHT_ALPHA, 2, 16, QChar('0'))
                                                         .arg(qRed(currentColor), 2, 16, QChar('0'))
                                                         .arg(qGreen(currentColor), 2, 16, QChar('0'))
                                                         .arg(qBlue(currentColor), 2, 16, QChar('0'));

            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_COLOR_HIGHLIGHT, newHighlightColor);
        }

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// On Font Size Spinner Value Changed Slot
//==============================================================================
void PreferencesDialog::on_fontSizeSpinner_valueChanged(int arg1)
{
    // Check Visibility
    if (isVisible()) {
        qDebug() << "PreferencesDialog::on_fontSizeSpinner_valueChanged - arg1: " << arg1;

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_FONT_SIZE, arg1);
        }

        // Set Dirty State
        setDirty();

        // ...
    }
}

//==============================================================================
// On Row Height Spinner Value Changed Slot
//==============================================================================
void PreferencesDialog::on_rowHeightSpinner_valueChanged(int arg1)
{
    // Check Visibility
    if (isVisible()) {
        qDebug() << "PreferencesDialog::on_rowHeightSpinner_valueChanged - arg1: " << arg1;

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_ROW_SIZE, arg1);
        }

        // Set Dirty State
        setDirty();

        // ...
    }
}

//==============================================================================
// On Alternate Row Colors Toggled Slot
//==============================================================================
void PreferencesDialog::on_alternateRowColorsCheckBox_toggled(bool checked)
{
    // Check Visibility
    if (isVisible()) {
        qDebug() << "PreferencesDialog::on_alternateRowColorsCheckBox_toggled - checked: " << checked;

        // Check Color Preview Context
        if (mColorsQMLContext) {
            // Set Context Property
            mColorsQMLContext->setContextProperty(COLORS_PREVIEW_CONTEXT_PROPERTY_ALT_ROWS, checked);
        }

        // Set Dirty State
        setDirty();

        // ...
    }
}

//==============================================================================
// Destructor
//==============================================================================
PreferencesDialog::~PreferencesDialog()
{
    // Shut Down
    shutDown();

    // ...

    // Delete UI
    delete ui;

    // Check Preferences
    if (mPreferences) {
        // Release Instance
        mPreferences->release();
    }

    qDebug() << "PreferencesDialog deleted.";
}

