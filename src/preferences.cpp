#include <QDebug>
#include <QQmlEngine>
#include <QDir>

#include "preferences.h"
#include "constants.h"
#include "setting_keys.h"
#include "settings_defaults.h"


// Preferences Singleton Instance
static Preferences* preferencesInstance = nullptr;

//==============================================================================
// Get Instance - Static Constructor
//==============================================================================
Preferences* Preferences::getInstance()
{
    // Check Singleton Instance
    if (!preferencesInstance) {
        // Create Singleton Instance
        preferencesInstance = new Preferences();
    } else {
        // Inc Reference Count
        preferencesInstance->mRefCount++;
    }

    return preferencesInstance;
}

//==============================================================================
// Release Instance
//==============================================================================
void Preferences::release()
{
    // Dec Ref Count
    mRefCount--;
    // Check Reference Count
    if (mRefCount <= 0) {
        // Delete Singleton Instance
        delete preferencesInstance;
        // Reset Singleton Instance Pointer
        preferencesInstance = nullptr;
    }
}

//==============================================================================
// Constructor
//==============================================================================
Preferences::Preferences(QObject* aParent)
    : QObject(aParent)
    , mRefCount(1)
    , mSettings(nullptr)
    , mDirty(false)
    , mGUITheme(SETTINGS_DEFAULT_GUI_THEME)
    , mGUIFontName(SETTINGS_DEFAULT_GUI_FONTNAME)
    , mGUIFontSize(SETTINGS_DEFAULT_GUI_FONTSIZE)
    , mListRowHeight(SETTINGS_DEFAULT_GUI_ROWHEIGHT)
    , mAlternateRowColors(true)
    , mColorText(SETTINGS_DEFAULT_COLOR_TEXT)
    , mColorBackground(SETTINGS_DEFAULT_COLOR_BACKGROUND)
    , mColorBorder(SETTINGS_DEFAULT_COLOR_BORDER)
    , mColorRowAlt(SETTINGS_DEFAULT_COLOR_ROW_ALT)
    , mColorHeader(SETTINGS_DEFAULT_COLOR_HEADER)
    , mColorHighlight(SETTINGS_DEFAULT_COLOR_HIGHLIGHT)
    , mDefaultFontDir(QDir::homePath())
    , mQtDir(QDir::homePath())
    , mColumnWidths(QString("64;256;64;64;256;256").split(";"))
    , mPreviewWidth(SETTINGS_DEFAULT_DEFAULT_PREVIEW_WIDTH)
    , mPlugins()
    , mLastLanguage("")
    , mLastWindowState(SETTINGS_DEFAULT_LAST_WINDOW_STATE)
    , mLastWindowWidth(SETTINGS_DEFAULT_LAST_WINDOW_WIDTH)
    , mLastWindowHeight(SETTINGS_DEFAULT_LAST_WINDOW_HEIGHT)
    , mSortingColumn(SETTINGS_DEFAULT_SORTING_COLUMN)
    , mSortingOrder(SETTINGS_DEFAULT_SORTING_ORDER)
{
    qDebug() << "Preferences created.";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void Preferences::init()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Create Settings
    mSettings = new QSettings();

    // Load Settings
    loadSettings();
}

//==============================================================================
// Load
//==============================================================================
void Preferences::loadSettings()
{
    qDebug() << "Preferences::loadSettings";

    // Set GUI Theme
    setGuiTheme(mSettings->value(SETTINGS_KEY_GUI_THEME, SETTINGS_DEFAULT_GUI_THEME).toInt());
    // Set GUI Font Name
    setGuiFontName(mSettings->value(SETTINGS_KEY_GUI_FONTNAME, SETTINGS_DEFAULT_GUI_FONTNAME).toString());
    // Set GUI Font Size
    setGuiFontSize(mSettings->value(SETTINGS_KEY_GUI_FONTSIZE, SETTINGS_DEFAULT_GUI_FONTSIZE).toInt());
    // Set List Row Size
    setListRowHeight(mSettings->value(SETTINGS_KEY_GUI_ROWHEIGHT, SETTINGS_DEFAULT_GUI_ROWHEIGHT).toInt());
    // Set Alternate Row Colors
    setAlternateRowColors(mSettings->value(SETTINGS_KEY_ALTERNATE_ROW_COLORS, SETTINGS_DEFAULT_ALTERNATE_ROWCOLORS).toBool());

    // Set Colors
    setColorText(mSettings->value(SETTINGS_KEY_COLOR_TEXT, SETTINGS_DEFAULT_COLOR_TEXT).toString());
    setColorBackground(mSettings->value(SETTINGS_KEY_COLOR_BACKGROUND, SETTINGS_DEFAULT_COLOR_BACKGROUND).toString());
    setColorBorder(mSettings->value(SETTINGS_KEY_COLOR_BORDER, SETTINGS_DEFAULT_COLOR_BORDER).toString());
    setColorRowAlt(mSettings->value(SETTINGS_KEY_COLOR_ROW_ALT, SETTINGS_DEFAULT_COLOR_ROW_ALT).toString());
    setColorHeader(mSettings->value(SETTINGS_KEY_COLOR_HEADER, SETTINGS_DEFAULT_COLOR_HEADER).toString());
    setColorHighlight(mSettings->value(SETTINGS_KEY_COLOR_HIGHLIGHT, SETTINGS_DEFAULT_COLOR_HIGHLIGHT).toString());

    // Set Default Font Dir
    setDefaultFontDir(mSettings->value(SETTINGS_KEY_DEFAULT_FONT_DIR).toString());
    // Set Qt Dir
    setQtDir(mSettings->value(SETTINGS_KEY_QT_DIR).toString());
    // Set Column Widths
    setColumnWidths(mSettings->value(SETTINGS_KEY_TRANSLATION_LIST_COLUMN_WIDTHS).toString().split(";", QString::SkipEmptyParts));
    // Set Preview Pane Width
    setPreviewWidth(mSettings->value(SETTINGS_KEY_PREVIEW_PANE_WIDTH, SETTINGS_DEFAULT_DEFAULT_PREVIEW_WIDTH).toInt());
    // Set Plugins
    setPluginsList(mSettings->value(SETTINGS_KEY_PLUGINS).toStringList());

    // Set Last Preview Language
    setLastLanguage(mSettings->value(SETTINGS_KEY_LAST_PREVIEW_LANGUAGE).toString());

    // Set Last Window State
    setLastWindowState(mSettings->value(SETTINGS_KEY_LAST_WINDOW_STATE, SETTINGS_DEFAULT_LAST_WINDOW_STATE).toInt());
    // Set Last Window Width
    setLastWindowWidth(mSettings->value(SETTINGS_KEY_LAST_WINDOW_WIDTH, SETTINGS_DEFAULT_LAST_WINDOW_WIDTH).toInt());
    // Set Last Window Height
    setLastWindowHeight(mSettings->value(SETTINGS_KEY_LAST_WINDOW_HEIGHT, SETTINGS_DEFAULT_LAST_WINDOW_HEIGHT).toInt());

    // Set Sorting Column
    setSortingColumn(mSettings->value(SETTINGS_KEY_SORTING_COLUMN, SETTINGS_DEFAULT_SORTING_COLUMN).toInt());
    // Set Sorting Order
    setSortingOrder(mSettings->value(SETTINGS_KEY_SORTING_ORDER, SETTINGS_DEFAULT_SORTING_ORDER).toBool());

    // ...
}

//==============================================================================
// Load Defualts
//==============================================================================
void Preferences::loadDefaults()
{
    qDebug() << "Preferences::loadDefaults";

    // Clear Settings
    mSettings->clear();

    // Set GUI Theme
    setGuiTheme(SETTINGS_DEFAULT_GUI_THEME);
    // Set Default GUI Font Name
    setGuiFontName(SETTINGS_DEFAULT_GUI_FONTNAME);
    // Set Default GUI Font Size
    setGuiFontSize(SETTINGS_DEFAULT_GUI_FONTSIZE);
    // Set Default Row Height
    setListRowHeight(SETTINGS_DEFAULT_GUI_ROWHEIGHT);
    // Set Default Alternate Row Colors
    setAlternateRowColors(SETTINGS_DEFAULT_ALTERNATE_ROWCOLORS);

    // Set Colors
    setColorText(SETTINGS_DEFAULT_COLOR_TEXT);
    setColorBackground(SETTINGS_DEFAULT_COLOR_BACKGROUND);
    setColorBorder(SETTINGS_DEFAULT_COLOR_BORDER);
    setColorRowAlt(SETTINGS_DEFAULT_COLOR_ROW_ALT);
    setColorHeader(SETTINGS_DEFAULT_COLOR_HEADER);
    setColorHighlight(SETTINGS_DEFAULT_COLOR_HIGHLIGHT);

    // Set Default Font Dir
    setDefaultFontDir(QDir::homePath());
    // Set Default Qt Dir
    setQtDir(QDir::homePath());
    // Set Default Preview Pane Width
    setPreviewWidth(SETTINGS_DEFAULT_DEFAULT_PREVIEW_WIDTH);

    // Clear Column Widths
    mColumnWidths.clear();

    // Set Column Widths
    mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_COLUMN_WIDTH0);
    mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_COLUMN_WIDTH1);
    mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_COLUMN_WIDTH2);
    mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_COLUMN_WIDTH3);
    mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_COLUMN_WIDTH4);
    mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_COLUMN_WIDTH5);

//    // Set Column Widths
//    setColumnWidth(0, SETTINGS_DEFAULT_COLUMN_WIDTH0);
//    setColumnWidth(1, SETTINGS_DEFAULT_COLUMN_WIDTH1);
//    setColumnWidth(2, SETTINGS_DEFAULT_COLUMN_WIDTH2);
//    setColumnWidth(3, SETTINGS_DEFAULT_COLUMN_WIDTH3);
//    setColumnWidth(4, SETTINGS_DEFAULT_COLUMN_WIDTH4);
//    setColumnWidth(5, SETTINGS_DEFAULT_COLUMN_WIDTH5);

    // Clear PLugins
    //clearPluginList();

//    // Set Last Preview Language
//    setLastLanguage(SETTINGS_DEFAULT_LAST_PREVIEW_LANGUAGE);
//    // Set Last Window State
//    setLastWindowState(SETTINGS_DEFAULT_LAST_WINDOW_STATE);
//    // Set Last Window Width
//    setLastWindowWidth(SETTINGS_DEFAULT_LAST_WINDOW_WIDTH);
//    // Set Last Window Height
//    setLastWindowHeight(SETTINGS_DEFAULT_LAST_WINDOW_HEIGHT);

    // Set Sorting Column
    setSortingColumn(SETTINGS_DEFAULT_SORTING_COLUMN);
    // Set Sorting Order
    setSortingOrder(SETTINGS_DEFAULT_SORTING_ORDER);

    // Emit Defaults Loaded Signal
    emit defaultsLoaded();

    // ...
}

//==============================================================================
// Save
//==============================================================================
void Preferences::saveSettings()
{
    // Check If Dirty
    if (mDirty) {
        qDebug() << "Preferences::saveSettings";

        // Save GUI Theme
        mSettings->setValue(SETTINGS_KEY_GUI_THEME, mGUITheme);
        // Save GUI Font Name
        mSettings->setValue(SETTINGS_KEY_GUI_FONTNAME, mGUIFontName);
        // Save GUI Font Size
        mSettings->setValue(SETTINGS_KEY_GUI_FONTSIZE, mGUIFontSize);
        // Save List Row Size
        mSettings->setValue(SETTINGS_KEY_GUI_ROWHEIGHT, mListRowHeight);
        // Save Alternate Row Colors
        mSettings->setValue(SETTINGS_KEY_ALTERNATE_ROW_COLORS, mAlternateRowColors);

        // Save Colors
        mSettings->setValue(SETTINGS_KEY_COLOR_TEXT, mColorText);
        mSettings->setValue(SETTINGS_KEY_COLOR_BACKGROUND, mColorBackground);
        mSettings->setValue(SETTINGS_KEY_COLOR_BORDER, mColorBorder);
        mSettings->setValue(SETTINGS_KEY_COLOR_ROW_ALT, mColorRowAlt);
        mSettings->setValue(SETTINGS_KEY_COLOR_HEADER, mColorHeader);
        mSettings->setValue(SETTINGS_KEY_COLOR_HIGHLIGHT, mColorHighlight);

        // Save Default Font Dir
        mSettings->setValue(SETTINGS_KEY_DEFAULT_FONT_DIR, mDefaultFontDir);

        // Save Qt Dir
        mSettings->setValue(SETTINGS_KEY_QT_DIR, mQtDir);

        // Save Columns Width
        mSettings->setValue(SETTINGS_KEY_TRANSLATION_LIST_COLUMN_WIDTHS, mColumnWidths.join(';'));

        // Save Preview Pane Width
        mSettings->setValue(SETTINGS_KEY_PREVIEW_PANE_WIDTH, mPreviewWidth);

        // Save Plugins
        mSettings->setValue(SETTINGS_KEY_PLUGINS, mPlugins);

        // Save Last Preview Language
        mSettings->setValue(SETTINGS_KEY_LAST_PREVIEW_LANGUAGE, mLastLanguage);
        // Save Last Window State
        mSettings->setValue(SETTINGS_KEY_LAST_WINDOW_STATE, mLastWindowState);
        // Save Last Window Width
        mSettings->setValue(SETTINGS_KEY_LAST_WINDOW_WIDTH, mLastWindowWidth);
        // Save Last Window Height
        mSettings->setValue(SETTINGS_KEY_LAST_WINDOW_HEIGHT, mLastWindowHeight);

        // Save Sorting Order
        mSettings->setValue(SETTINGS_KEY_SORTING_COLUMN, mSortingColumn);
        // Save Sorting Order
        mSettings->setValue(SETTINGS_KEY_SORTING_ORDER, mSortingOrder);

        // ...

        // Sync
        mSettings->sync();
        // Set Dirty State
        setDirty(false);
    }
}

//==============================================================================
// Clear
//==============================================================================
void Preferences::clear()
{
    qDebug() << "Preferences::clear";

    // Clear Plugin List
    clearPluginList();

    // Clear
    //mSettings->clear();
}

//==============================================================================
// Set Dirty State
//==============================================================================
void Preferences::setDirty(const bool& aDirty)
{
    // Check Dirty State
    if (mDirty != aDirty) {
        // Set Dirty State
        mDirty = aDirty;
        // Emit Dirty State Changed Signal
        emit dirtyChanged(mDirty);
    }
}

//==============================================================================
// Shut Down
//==============================================================================
void Preferences::shutDown()
{
    qDebug() << "Preferences::shutDown";

    // Clear
    clear();
}

//==============================================================================
// Get Dirty State
//==============================================================================
bool Preferences::isDirty()
{
    return mDirty;
}

//==============================================================================
// Get GUI Theme
//==============================================================================
int Preferences::guiTheme()
{
    return mGUITheme;
}

//==============================================================================
// Set GUI Theme
//==============================================================================
void Preferences::setGuiTheme(const int& aTheme)
{
    // Check GUI Theme
    if (mGUITheme != aTheme) {
        // Set GUI Theme
        mGUITheme = aTheme;
        // Emit GUI Theme Changed Signal
        emit guiThemeChanged(mGUITheme);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get GUI Font Name
//==============================================================================
QString Preferences::guiFontName()
{
    return mGUIFontName;
}

//==============================================================================
// Set GUI Font Name
//==============================================================================
void Preferences::setGuiFontName(const QString& aFontName)
{
    // Check GUI Font Name
    if (mGUIFontName != aFontName) {
        // Set GUI Font Name
        mGUIFontName = aFontName;
        // Emit GUI Font Name Changed Signal
        emit guiFontNameChanged(mGUIFontName);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get GUI Font Size
//==============================================================================
int Preferences::guiFontSize()
{
    //return
    return mGUIFontSize;
}

//==============================================================================
// Set GUI Font Size
//==============================================================================
void Preferences::setGuiFontSize(const int& aFontSize)
{
    // Check GUI Font Size
    if (mGUIFontSize != aFontSize) {
        // Set GUI Font Size
        mGUIFontSize = aFontSize;
        // Emit GUI Font Size Changed Signal
        emit guiFontSizeChanged(mGUIFontSize);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get List Row Size
//==============================================================================
int Preferences::listRowHeight()
{
    return mListRowHeight;
}

//==============================================================================
// Set List Row Size
//==============================================================================
void Preferences::setListRowHeight(const int& aRowHeight)
{
    // Check List Row Size
    if (mListRowHeight != aRowHeight) {
        // Set List Row Size
        mListRowHeight = aRowHeight;
        // Emit List Row Size Changed Signal
        emit listRowHeightChanged(mListRowHeight);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Alternate Row Colors
//==============================================================================
bool Preferences::alternateRowColors()
{
    return mAlternateRowColors;
}

//==============================================================================
// Set Alternate Row Colors
//==============================================================================
void Preferences::setAlternateRowColors(const bool& aAlternateRowColors)
{
    // Check Setting
    if (mAlternateRowColors != aAlternateRowColors) {
        // Set Alternate Row Colors
        mAlternateRowColors = aAlternateRowColors;
        // Emit Alternate Row Colors Changed Signal
        emit alternateRowColorsChanged(mAlternateRowColors);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Color Text
//==============================================================================
QString Preferences::colorText()
{
    return mColorText;
}

//==============================================================================
// Set Color Text
//==============================================================================
void Preferences::setColorText(const QString& aColorText)
{
    // Check Color Text
    if (mColorText != aColorText) {
        // Set Color Text
        mColorText = aColorText;
        // Emit Color Taxt Changed Signal
        emit colorTextChanged(mColorText);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Color Background
//==============================================================================
QString Preferences::colorBackground()
{
    return mColorBackground;
}

//==============================================================================
// Set Color Background
//==============================================================================
void Preferences::setColorBackground(const QString& aColorBackground)
{
    // Check Color Background
    if (mColorBackground != aColorBackground) {
        // Set Color Background
        mColorBackground = aColorBackground;
        // Emit Color Background Changes Signal
        emit colorBackgroundChanged(mColorBackground);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Color Border
//==============================================================================
QString Preferences::colorBorder()
{
    return mColorBorder;
}

//==============================================================================
// Set Color Border
//==============================================================================
void Preferences::setColorBorder(const QString& aColorBorder)
{
    // Check Color Border
    if (mColorBorder != aColorBorder) {
        // Set Color Border
        mColorBorder = aColorBorder;
        // Emit Color Border Changed Signal
        emit colorBorderChanged(mColorBorder);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Color Row Alternative
//==============================================================================
QString Preferences::colorRowAlt()
{
    return mColorRowAlt;
}

//==============================================================================
// Set Color Row Alternative
//==============================================================================
void Preferences::setColorRowAlt(const QString& aColorRowAlt)
{
    // Check Color Row Alternative
    if (mColorRowAlt != aColorRowAlt) {
        // Set Color Row Alternative
        mColorRowAlt = aColorRowAlt;
        // Emit Color Row Alternative Changed Signal
        emit colorRowAltChanged(mColorRowAlt);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Color Header
//==============================================================================
QString Preferences::colorHeader()
{
    return mColorHeader;
}

//==============================================================================
// Set Color Header
//==============================================================================
void Preferences::setColorHeader(const QString& aColorHeader)
{
    // Check Color Header
    if (mColorHeader != aColorHeader) {
        // Set Color Header
        mColorHeader = aColorHeader;
        // Emit Color Header Changed Signal
        emit colorHeaderChanged(mColorHeader);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Color Highlight
//==============================================================================
QString Preferences::colorHighlight()
{
    return mColorHighlight;
}

//==============================================================================
// Set Color Highlight
//==============================================================================
void Preferences::setColorHighlight(const QString& aColorHighlight)
{
    // Check Color Highlight
    if (mColorHighlight != aColorHighlight) {
        // Set Color Highlight
        mColorHighlight = aColorHighlight;
        // Emit Color Highlight Changed Signal
        emit colorHighlightChanged(mColorHighlight);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Default Font Dir
//==============================================================================
QString Preferences::defaultFontDir()
{
    return mDefaultFontDir;
}

//==============================================================================
// Set Default Font Dir
//==============================================================================
void Preferences::setDefaultFontDir(const QString& aDefaultFontDir)
{
    // Check Default Font Dir
    if (mDefaultFontDir != aDefaultFontDir) {
        // Set Default Font Dir
        mDefaultFontDir = aDefaultFontDir;
        // Emit Default Font Dir Changed Signal
        emit defaultFontDirChanged(mDefaultFontDir);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Qt Dir
//==============================================================================
QString Preferences::qtDir()
{
    return mQtDir;
}

//==============================================================================
// Set Qt Dir
//==============================================================================
void Preferences::setQtDir(const QString& aQtDir)
{
    // Check Qt Dir
    if (mQtDir != aQtDir) {
        // Set Qt Dir
        mQtDir = aQtDir;
        // Emit Qt Dir Changed Signal
        emit qtDirChanged(mQtDir);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Set Column Widths
//==============================================================================
void Preferences::setColumnWidths(const QStringList& aColumnWidths)
{
    // Check Column Widths
    if (mColumnWidths != aColumnWidths) {
        // Set Column Widths
        mColumnWidths = aColumnWidths;
        // Emit Column Widths Changed
        emit columnWidthsChanged(mColumnWidths);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Set Plugins List
//==============================================================================
void Preferences::setPluginsList(const QStringList& aPlugins)
{
    // Check Plugin List
    if (mPlugins != aPlugins) {
        // Set Plugins List
        mPlugins = aPlugins;
        // Emit Plugins List Changed Signal
        emit pluginListChanged(mPlugins);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Default Column Width
//==============================================================================
int Preferences::defaultColumnWidth()
{
    return SETTINGS_DEFAULT_DEFAULT_COLUMN_WIDTH;
}

//==============================================================================
// Get Column Widths
//==============================================================================
QStringList Preferences::columnWidths()
{
    return mColumnWidths;
}

//==============================================================================
// Get Column Width
//==============================================================================
int Preferences::getColumnWidth(const int& aIndex)
{
    // Get Column Widths
    QStringList colWidths = columnWidths();

    // Check Index
    if (aIndex >= 0 && aIndex < colWidths.count()) {
        return colWidths[aIndex].toInt();
    }

    return SETTINGS_DEFAULT_DEFAULT_COLUMN_WIDTH;
}

//==============================================================================
// Set Column Width
//==============================================================================
void Preferences::setColumnWidth(const int& aIndex, const int& aWidth)
{
    // Check Index
    if (aIndex >= 0 && aIndex < mColumnWidths.count()) {
        // Get Column Width
        int colWidth = mColumnWidths[aIndex].toInt();

        // Check Width
        if (colWidth != aWidth) {
            //qDebug() << "Preferences::setColumnWidth - aIndex: " << aIndex << " - aWidth: " << aWidth;

            // Set Column Width
            mColumnWidths[aIndex] = QString("%1").arg(aWidth);

            // Emit Column Width Changed
            emit columnWidthChanged(aIndex, aWidth);

            // Set Dirty State
            setDirty();
        }
    // Check Index
    } else if (aIndex >=0) {
        // Set Column Widths
        while (mColumnWidths.count() - 1 < aIndex) {
            // Set Column Width
            mColumnWidths << QString("%1").arg(SETTINGS_DEFAULT_DEFAULT_COLUMN_WIDTH);
        }

        // Add Column Width
        mColumnWidths << QString("%1").arg(aWidth);

        // Emit Column Width Changed Signal
        emit columnWidthChanged(aIndex, aWidth);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Preview Pane Width
//==============================================================================
int Preferences::previewWidth()
{
    return mPreviewWidth;
}

//==============================================================================
// Set Preview Pane Width
//==============================================================================
void Preferences::setPreviewWidth(const int& aNewWidth)
{
    // Check Preview Pane Width
    if (mPreviewWidth != aNewWidth) {
        // Set Preview Pane
        mPreviewWidth = aNewWidth;
        // Emit Preview Pane Width Changed Signal
        emit previewWidthChanged(mPreviewWidth);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Plugin List
//==============================================================================
QStringList Preferences::pluginList()
{
    return mPlugins;
}

//==============================================================================
// Add Plugin
//==============================================================================
void Preferences::addPlugin(const QString& aPath)
{
    // Check Path
    if (!aPath.isEmpty()) {
        // Get Path Index
        int pIndex = mPlugins.indexOf(aPath);

        // Check Path Index
        if (pIndex == -1) {
            qDebug() << "Preferences::addPlugin - aPath: " << aPath;

            // Append Path
            mPlugins << aPath;
            // Emit Plugins List Changed Signal
            emit pluginListChanged(mPlugins);

            // Set Dirty State
            setDirty();
        }
    } else {
        qWarning() << "Preferences::addPlugin - EMPTY PLUGIN PATH!";
    }
}

//==============================================================================
// Remove Plugin
//==============================================================================
void Preferences::removePlugin(const int& aIndex)
{
    // Check Index
    if (aIndex >=0 && aIndex < mPlugins.count()) {
        // Remove Plugin
        mPlugins.removeAt(aIndex);
        // Emit Plugins List Changed Signal
        emit pluginListChanged(mPlugins);

        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Clear Plugin List
//==============================================================================
void Preferences::clearPluginList()
{
    qDebug() << "Preferences::clearPluginList";

    // Clear
    mPlugins.clear();
    // Emit Plugins List Changed Signal
    emit pluginListChanged(mPlugins);

    // Set Dirty State
    setDirty();
}

//==============================================================================
// Get Last Used Preview Language
//==============================================================================
QString Preferences::lastLanguage()
{
    return mLastLanguage;
}

//==============================================================================
// Set Last Used Preview Language
//==============================================================================
void Preferences::setLastLanguage(const QString& aLanguage)
{
    // Check Last Language
    if (mLastLanguage != aLanguage) {
        // Set Last Language
        mLastLanguage = aLanguage;
        // Emit Last Preview Language Changed Signal
        emit lastLanguageChanged(mLastLanguage);
        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Last Window State
//==============================================================================
int Preferences::lastWindowState()
{
    return mLastWindowState;
}

//==============================================================================
// Set Last Window State
//==============================================================================
void Preferences::setLastWindowState(const int& aState)
{
    // Check Last Window State
    if (mLastWindowState != aState) {
        // Set Last Window State
        mLastWindowState = aState;
        // Emit Last Window State Changed Signal
        emit lastWindowStateChanged(mLastWindowState);
        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Last Window Width
//==============================================================================
int Preferences::lastWindowWidth()
{
    return mLastWindowWidth;
}

//==============================================================================
// Set Last Window Width
//==============================================================================
void Preferences::setLastWindowWidth(const int& aWidth)
{
    // Check Last Width
    if (mLastWindowWidth != aWidth) {
        // Set Last Width
        mLastWindowWidth = aWidth;
        // Emit Last Width Changed Signal
        emit lastWindowWidthChanged(mLastWindowWidth);
        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Last Window Height
//==============================================================================
int Preferences::lastWindowHeight()
{
    return mLastWindowHeight;
}

//==============================================================================
// Set Last Window Height
//==============================================================================
void Preferences::setLastWindowHeight(const int& aHeight)
{
    // Check Last Height
    if (mLastWindowHeight != aHeight) {
        // Set Last Height
        mLastWindowHeight = aHeight;
        // Emit Last Height Changed Signal
        emit lastWindowHeightChanged(mLastWindowHeight);
        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Sorting Column
//==============================================================================
int Preferences::sortingColumn()
{
    return mSortingColumn;
}

//==============================================================================
// Set Sorting Column
//==============================================================================
void Preferences::setSortingColumn(const int& aSortingColumn)
{
    // Check Sorting Column
    if (mSortingColumn != aSortingColumn) {
        // Set Sorting Column
        mSortingColumn = aSortingColumn;
        // Emit Sorting Column Changed Signal
        emit sortingColumnChanged(mSortingColumn);
        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Get Sorting Order
//==============================================================================
bool Preferences::sortingOrder()
{
    return mSortingOrder;
}

//==============================================================================
// Set Sorting Order
//==============================================================================
void Preferences::setSortingOrder(const bool& aSortingOrder)
{
    // Check Sorting Order
    if (mSortingOrder != aSortingOrder) {
        // Set Sorting Order
        mSortingOrder = aSortingOrder;
        // Emit Sorting Order Changed Signal
        emit sortingOrderChanged(mSortingOrder);
        // Set Dirty State
        setDirty();
    }
}

//==============================================================================
// Destructor
//==============================================================================
Preferences::~Preferences()
{
    // Save Settings
    saveSettings();
    // Shut Down
    shutDown();

    // Delete Settings
    delete mSettings;

    qDebug() << "Preferences deleted.";
}
