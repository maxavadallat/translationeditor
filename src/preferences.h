#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QString>
#include <QSettings>

//==============================================================================
// Preferences Class
//==============================================================================
class Preferences : public QObject
{
    Q_OBJECT

    // Dirty State
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY dirtyChanged)

    // GUI Theme
    Q_PROPERTY(int guiTheme READ guiTheme WRITE setGuiTheme NOTIFY guiThemeChanged)
    // GUI Font Name
    Q_PROPERTY(QString guiFontName READ guiFontName WRITE setGuiFontName NOTIFY guiFontNameChanged)
    // GUI Font Size
    Q_PROPERTY(int guiFontSize READ guiFontSize WRITE setGuiFontSize NOTIFY guiFontSizeChanged)
    // List Row Size
    Q_PROPERTY(int listRowHeight READ listRowHeight WRITE setListRowHeight NOTIFY listRowHeightChanged)
    // Alternate Row Colors
    Q_PROPERTY(bool alternateRowColors READ alternateRowColors WRITE setAlternateRowColors NOTIFY alternateRowColorsChanged)

    // Colors
    Q_PROPERTY(QString colorText READ colorText WRITE setColorText NOTIFY colorTextChanged)
    Q_PROPERTY(QString colorBackground READ colorBackground WRITE setColorBackground NOTIFY colorBackgroundChanged)
    Q_PROPERTY(QString colorBorder READ colorBorder WRITE setColorBorder NOTIFY colorBorderChanged)
    Q_PROPERTY(QString colorRowAlt READ colorRowAlt WRITE setColorRowAlt NOTIFY colorRowAltChanged)
    Q_PROPERTY(QString colorHeader READ colorHeader WRITE setColorHeader NOTIFY colorHeaderChanged)
    Q_PROPERTY(QString colorHighlight READ colorHighlight WRITE setColorHighlight NOTIFY colorHighlightChanged)

    // Default Translation Item List Column Width
    Q_PROPERTY(int defaultColumnWidth READ defaultColumnWidth CONSTANT)
    // Translation List Column Widths
    Q_PROPERTY(QStringList columnWidths READ columnWidths NOTIFY columnWidthsChanged)
    // Preview Pane Width
    Q_PROPERTY(int previewWidth READ previewWidth WRITE setPreviewWidth NOTIFY previewWidthChanged)
    // Plugin List
    Q_PROPERTY(QStringList pluginList READ pluginList NOTIFY pluginListChanged)

    // Last Used Preview Language
    Q_PROPERTY(QString lastLanguage READ lastLanguage WRITE setLastLanguage NOTIFY lastLanguageChanged)

    // Default Font Dir
    Q_PROPERTY(QString defaultFontDir READ defaultFontDir WRITE setDefaultFontDir NOTIFY defaultFontDirChanged)

    // Qt Dir
    Q_PROPERTY(QString qtDir READ qtDir WRITE setQtDir NOTIFY qtDirChanged)

    // Last Window State
    Q_PROPERTY(int lastWindowState READ lastWindowState WRITE setLastWindowState NOTIFY lastWindowStateChanged)
    // Last Window Width
    Q_PROPERTY(int lastWindowWidth READ lastWindowWidth WRITE setLastWindowWidth NOTIFY lastWindowWidthChanged)
    // Last Window Height
    Q_PROPERTY(int lastWindowHeight READ lastWindowHeight WRITE setLastWindowHeight NOTIFY lastWindowHeightChanged)

    // Sorting Column
    Q_PROPERTY(int sortingColumn READ sortingColumn WRITE setSortingColumn NOTIFY sortingColumnChanged)
    // Sorting Order
    Q_PROPERTY(bool sortingOrder READ sortingOrder WRITE setSortingOrder NOTIFY sortingOrderChanged)

public:
    // Get Instance - Static Constructor
    static Preferences* getInstance();
    // Release Instance
    void release();

    // Load Defualts
    void loadDefaults();

    // Get Dirty State
    bool isDirty();

    // Get GUI Theme
    int guiTheme();
    // Set GUI Theme
    void setGuiTheme(const int& aTheme);

    // Get GUI Font Name
    QString guiFontName();
    // Set GUI Font Name
    void setGuiFontName(const QString& aFontName);

    // Get GUI Font Size
    int guiFontSize();
    // Set GUI Font Size
    void setGuiFontSize(const int& aFontSize);

    // Get List Row Size
    int listRowHeight();
    // Set List Row Size
    void setListRowHeight(const int& aRowHeight);

    // Get Alternate Row Colors
    bool alternateRowColors();
    // Set Alternate Row Colors
    void setAlternateRowColors(const bool& aAlternateRowColors);

    // Get Color Text
    QString colorText();
    // Set Color Text
    void setColorText(const QString& aColorText);

    // Get Color Background
    QString colorBackground();
    // Set Color Background
    void setColorBackground(const QString& aColorBackground);

    // Get Color Border
    QString colorBorder();
    // Set Color Border
    void setColorBorder(const QString& aColorBorder);

    // Get Color Row Alternative
    QString colorRowAlt();
    // Set Color Row Alternative
    void setColorRowAlt(const QString& aColorRowAlt);

    // Get Color Header
    QString colorHeader();
    // Set Color Header
    void setColorHeader(const QString& aColorHeader);

    // Get Color Highlight
    QString colorHighlight();
    // Set Color Highlight
    void setColorHighlight(const QString& aColorHighlight);

    // Get Default Font Dir
    QString defaultFontDir();
    // Set Default Font Dir
    void setDefaultFontDir(const QString& aDefaultFontDir);

    // Get Qt Dir
    QString qtDir();
    // Set Qt Dir
    void setQtDir(const QString& aQtDir);

    // Get Default Column Width
    int defaultColumnWidth();

    // Get Column Widths
    QStringList columnWidths();

    // Get Column Width
    Q_INVOKABLE int getColumnWidth(const int& aIndex);
    // Set Column Width
    Q_INVOKABLE void setColumnWidth(const int& aIndex, const int& aWidth);

    // Get Preview Pane Width
    int previewWidth();
    // Set Preview Pane Width
    void setPreviewWidth(const int& aNewWidth);

    // Plugin List
    QStringList pluginList();

    // Add Plugin
    void addPlugin(const QString& aPath);
    // Remove Plugin
    void removePlugin(const int& aIndex);

    // Clear Plugin List
    void clearPluginList();

    // Get Last Used Preview Language
    QString lastLanguage();
    // Set Last Used Preview Language
    void setLastLanguage(const QString& aLanguage);

    // Get Last Window State
    int lastWindowState();
    // Set Last Window State
    void setLastWindowState(const int& aState);

    // Get Last Window Width
    int lastWindowWidth();
    // Set Last Window Width
    void setLastWindowWidth(const int& aWidth);

    // Get Last Window Height
    int lastWindowHeight();
    // Set Last Window Height
    void setLastWindowHeight(const int& aHeight);

    // Get Sorting Column
    int sortingColumn();
    // Set Sorting Column
    void setSortingColumn(const int& aSortingColumn);

    // Get Sorting Order
    bool sortingOrder();
    // Set Sorting Order
    void setSortingOrder(const bool& aSortingOrder);

signals:
    // Dirty State changed Signal
    void dirtyChanged(const bool& aDirty);
    // Defaults Loaded Signa;
    void defaultsLoaded();
    // GUI Theme Changed Signal
    void guiThemeChanged(const int& aTheme);
    // GUI Font Name Changed Signal
    void guiFontNameChanged(const QString& aFontName);
    // GUI Font Size Changed Signal
    void guiFontSizeChanged(const int& newFontSize);
    // List Row Size Changed Signal
    void listRowHeightChanged(const int& newRowHeight);
    // Alternate Row Colors Changed Signal
    void alternateRowColorsChanged(const bool& newAlternateRowColors);
    // Color Text Changed Signal
    void colorTextChanged(const QString& aColorText);
    // Color Background Changed Signal
    void colorBackgroundChanged(const QString& aColorBackground);
    // Color Border Changed Signal
    void colorBorderChanged(const QString& aColorBorder);
    // Color Row Alternative Changed Signal
    void colorRowAltChanged(const QString& aColorRowAlt);
    // Color Header Changed Signal
    void colorHeaderChanged(const QString& aColorHeader);
    // Color Highlight Changed Signal
    void colorHighlightChanged(const QString& aColorHighlight);
    // Default Font Dir Changed Signal
    void defaultFontDirChanged(const QString& newDefaultFontDir);
    // Qt Dir Changed Signal
    void qtDirChanged(const QString& aQtDir);
    // Translation Item Column Width Changed Signal
    void columnWidthChanged(const int& aIndex, const int& aWidth);
    // Translation Item List Column Widths Changed Signal
    void columnWidthsChanged(const QStringList& newColumns);
    // Preview Width Changed Signal
    void previewWidthChanged(const int& newWidth);
    // Plug-in List Changed Signal
    void pluginListChanged(const QStringList& aPluginList);
    // Last Used Preview Language Changed Signal
    void lastLanguageChanged(const QString& aLanguage);
    // Last Window State Changed Signal
    void lastWindowStateChanged(const int& aState);
    // Last Window Width Changed Signal
    void lastWindowWidthChanged(const int& aWidth);
    // Last Window Height Changed Signal
    void lastWindowHeightChanged(const int& aHeight);
    // Sorting Column Changed Signal
    void sortingColumnChanged(const int& aSortingColumn);
    // Sorting Order Changed Signal
    void sortingOrderChanged(const bool& aSortingOrder);

protected:
    friend class PreferencesDialog;

    // Constructor
    explicit Preferences(QObject* aParent = nullptr);
    // Destructor
    ~Preferences();

    // Init
    void init();

    // Load
    void loadSettings();
    // Save
    void saveSettings();
    // Clear
    void clear();

    // Set Dirty State
    void setDirty(const bool& aDirty = true);

    // Set Column Widths
    void setColumnWidths(const QStringList& aColumnWidths);

    // Set Plugins List
    void setPluginsList(const QStringList& aPlugins);

    // Shut Down
    void shutDown();

protected:
    // Reference Count
    int                 mRefCount;
    // Settings Storage
    QSettings*          mSettings;

    // Dirty State
    bool                mDirty;

    // GUI Theme
    int                 mGUITheme;
    // Gui Font Name
    QString             mGUIFontName;
    // GUI Font Size
    int                 mGUIFontSize;
    // List Row Size
    int                 mListRowHeight;
    // Alternate Row Colors
    bool                mAlternateRowColors;

    // Colors
    QString             mColorText;
    QString             mColorBackground;
    QString             mColorBorder;
    QString             mColorRowAlt;
    QString             mColorHeader;
    QString             mColorHighlight;

    // Default Font Dir
    QString             mDefaultFontDir;

    // Qt Dir
    QString             mQtDir;

    // Column Widths
    QStringList         mColumnWidths;

    // Preview Pane Width
    int                 mPreviewWidth;

    // Plugin List
    QStringList         mPlugins;

    // Last Preview Language
    QString             mLastLanguage;

    // Last Window State
    int                 mLastWindowState;
    // Last Window Width
    int                 mLastWindowWidth;
    // Last Window Height
    int                 mLastWindowHeight;

    // Sorting Column
    int                 mSortingColumn;
    // Sorting Order
    bool                mSortingOrder;

    // ...
};

#endif // PREFERENCES_H
