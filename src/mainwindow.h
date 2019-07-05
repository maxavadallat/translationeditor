#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class Preferences;
class PreferencesDialog;
class PackagePreferencesDialog;
class AboutDialog;
class InfoDialog;
class ConfirmationDialog;
class HelpDialog;

class NewGroupDialog;
class NewTranslationDialog;
class AlternateFontsDialog;
class FindItemsDialog;

class PackageInfo;
class PackageGroup;
class TranslationItem;
class TranslationItemLayout;

class PluginInfo;

//==============================================================================
// Main Window Class
//==============================================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Current Package
    Q_PROPERTY(PackageInfo* currentPackage READ currentPackage NOTIFY currentPackageChanged)
    // Current Group
    Q_PROPERTY(PackageGroup* currentGroup READ currentGroup NOTIFY currentGroupChanged)
    // Current Translation Item
    Q_PROPERTY(TranslationItem* currentItem READ currentItem NOTIFY currentItemChanged)
    // Current Index
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    // Current Translation Item Layout
    Q_PROPERTY(TranslationItemLayout* currentLayout READ currentLayout NOTIFY currentLayoutChanged)
    // Recent Files
    Q_PROPERTY(QStringList recentFiles READ recentFiles NOTIFY recentFilesChanged)
    // Supported Languages
    Q_PROPERTY(QStringList supportedLanguages READ supportedLanguages CONSTANT)
    // Selected Translation Item Language
    Q_PROPERTY(QString selectedItemLanguage READ selectedItemLanguage NOTIFY selectedItemLanguageChanged)
    // Checking Mode Active
    Q_PROPERTY(bool checkingActive READ checkingActive NOTIFY checkingActiveChanged)
    // Checking Index
    Q_PROPERTY(int checkIndex READ checkIndex WRITE setCheckIndex NOTIFY checkIndexChanged)
    // Cell Edit Mode
    Q_PROPERTY(bool cellEditMode READ cellEditMode WRITE setCellEditMode NOTIFY cellEditModeChanged)
    // Busy
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    // Loaders Count
    Q_PROPERTY(int loadersCount READ loadersCount WRITE setLoadersCount NOTIFY loadersCountChanged)

public:
    // Constructor
    explicit MainWindow(QWidget* aParent = nullptr);

    // Get Current Package
    PackageInfo* currentPackage();
    // Get Current Group
    PackageGroup* currentGroup();
    // Get Current Translation Item
    TranslationItem* currentItem();
    // Get Current Index
    int currentIndex();
    // Get Current Translation Item Layout
    TranslationItemLayout* currentLayout();

    // Get Recent Files
    QStringList recentFiles();
    // Get Supported Languages
    QStringList supportedLanguages();
    // Get Selected Translation Item Language
    QString selectedItemLanguage();

    // Checking Mode Active
    bool checkingActive();

    // Get Checking Index
    int checkIndex();
    // Set Checking Index
    void setCheckIndex(const int& aIndex);

    // Get Cell Edit Mode
    bool cellEditMode();
    // Set Cell Edit Mode
    void setCellEditMode(const bool& aCellEditMode);

    // Get Busy State
    bool busy();

    // Get Loaders Count
    int loadersCount();
    // Set Loaders Count
    void setLoadersCount(const int& aCount);

    // Get Language Name
    Q_INVOKABLE QString getLanguageName(const QString& aLocale);
    // Get Language Flag URL
    Q_INVOKABLE QString getLanguageFlag(const QString& aLocale);

    // Show Info Dialog
    Q_INVOKABLE void showInfoDialog(const QString& aTitle, const QString& aInfoText);
    // Get Confirmation
    Q_INVOKABLE bool getConfirmation(const QString& aTitle, const QString& aConfirmText);

    // Open Package
    Q_INVOKABLE void loadPackage(const QString& aFilePath);

    // Add Translation Group
    Q_INVOKABLE void addTranslationGroup();
    // Remove Translation Group
    Q_INVOKABLE void removeTranslationGroup(const int& aIndex = -1);

    // Select Translation Item
    Q_INVOKABLE TranslationItem* selectTranslationItem(const int& aIndex);

    // Edit Translation Item Slot
    Q_INVOKABLE void editTranslationItem(const int& aIndex = -1);

    // Goto Previous Translation Layout
    Q_INVOKABLE void prevTranslationLayout();
    // Goto Next Translation Layout
    Q_INVOKABLE void nextTranslationLayout();

    // Set Selected Translation Language
    Q_INVOKABLE void setSelectedTranslationLanguage(const int& aIndex, const bool& aUpdateComboBox = false);

    // Get Selected Language Index
    Q_INVOKABLE int getSelectedLanguageIndex();

    // Set Current Layout
    Q_INVOKABLE void setCurrentLayout(TranslationItemLayout* aTranslationItemLayout);

    // Check Current Translation Item
    Q_INVOKABLE void checkTranslationItem();
    // Check All Translations
    Q_INVOKABLE void checkAllTranslations();
    // Stop Checking
    Q_INVOKABLE void stopCheckingAllTranslations(const bool& aAbort = false);
    // Finish Checking
    Q_INVOKABLE void finishCheckingAllTranslations(const bool& aSingleCheck = false);

    // Show Status Message
    Q_INVOKABLE void showStatusMessage(const QString& aMessage, const int& aTimeOut = 4000);

    // Get Last Window State
    int lastWindowState();

    // Destructor
    ~MainWindow();

signals:
    // Current Package Changed Signal
    void currentPackageChanged(PackageInfo* aPackage);
    // Current Group Changed Signal
    void currentGroupChanged(PackageGroup* aGroup);
    // Current Item Changed
    void currentItemChanged(TranslationItem* aTranslationItem);
    // Current Index Changed Signal
    void currentIndexChanged(const int& aIndex);
    // Serach Result Item Selected Signal
    void searchResultItemSelected(const int& aIndex);
    // Current Translation Item Layout Changed Signal
    void currentLayoutChanged(TranslationItemLayout* aTranslationItemLayout);
    // Recent Files Changed Signal
    void recentFilesChanged(const QStringList& aRecentFiles);
    // Selected Translation Item Language Changed Signal
    void selectedItemLanguageChanged(const QString& aLanguage);
    // Checking Mode Active State Changd Signl
    void checkingActiveChanged(const bool& aCheckingActive);
    // Checking Index Changed Signal
    void checkIndexChanged(const int& newCheckIndex);
    // Single Check Started Signal
    void singleCheckStarted();
    // Checking Started Signal
    void checkingStarted();
    // Checking Aborted Signal
    void checkingAborted();
    // Checking Finished Signal
    void checkingFinished(const bool& aSingleCheck);
    // Toggle Filter Editor
    void toggleFilterEditor();
    // Reset Sorting Signal
    void resetSorting();
    // Cell Edit Mode Changed Signal
    void cellEditModeChanged(const bool& aCellEditMode);
    // Busy State Changed Signal
    void busyChanged(const bool& aBusy);
    // Busy Text Changed Signal
    void busyTextChanged(const QString& newText);
    // Loaders Count Changed Signal
    void loadersCountChanged(const int& aCount);

public: // From QMainWindow
    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);
    // Resize Event
    virtual void resizeEvent(QResizeEvent* aEvent);
    // Close Ecent
    virtual void closeEvent(QCloseEvent* aEvent);

protected:
    // Init
    void init();
    // ShutDown
    void shutDown();
    // Restore UI
    void restoreUI();

    // Build Language Map
    void buildLanguageMap();

    // Create New Project
    void createNewPackage();
    // Load Package
    void loadPackage();
    // Save Package
    void savePackage();
    // Save Package As
    void saveAsPackage();
    // Close Package
    void closePackage();

    // Set Current Package
    void setCurrentPackage(PackageInfo* aPackage);
    // Set Current Group
    void setCurrentGroup(PackageGroup* aPackageGroup);
    // Set Current Index
    void setCurrentIndex(const int& aIndex);
    // Set Current Translation Item
    void setCurrentItem(TranslationItem* aTranslationItem);

    // Set Selected Translation Item Language
    void setSelectedItemLanguage(const QString& aLanguage);

    // Select All Translations
    void selectAllTranslations();
    // Select None Translations
    void selectNoneTranslations();

    // Add Translation Item
    void addTranslationItem();
    // Remove Translation Item
    void removeTranslationItem(const int& aIndex = -1);

    // Add Translation Layout
    void addTranslationLayout();
    // Remvoe Translation Layout
    void removeTranslationLayout(const int& aIndex = -1);

    // Set Current Translation Item Current Layout Max Width
    void setLayoutMaxWidth(const int& aWidth);
    // Set Current Translation Item Current Layout Max Height
    void setLayoutMaxHeight(const int& aHeight);
    // Set Current Translation Item Current Layout Max Lines
    void setLayoutMaxLines(const int& aLines);
    // Set Current Translation Item Max Length
    void setLayoutMaxLength(const int& aMaxLength);

    // Set Current Translation Item Current Layout Wrap Type
    void setLayoutWrapType(const int& aWrapType);
    // Set Current Translation Item Current Layout Elide Type
    void setLayoutElideType(const int& aElideType);

    // Set Current Translation Item Current Layout Font Name Index
    void setLayoutFontName(const int& aFontIndex);
    // Set Current Translation Item Current Layout Font Size
    void setLayoutFontSize(const int& aFontSize);

    // Set Layout Shrinkable
    void setLayoutShrinkable(const bool& aShrinkable);

    // Set Busy State
    void setBusy(const bool& aBusy);

    // Launch Find Dialog
    void launchFindDialog();

    // Launch Package Preferences
    void launchPackagePreferencesDialog(const bool& aNewPackage = false);

    // Launch Alternative Fonts Dialog
    void launchAlternativeFontsDialog();

    // Show Preview Pane
    void showPreviewPane();
    // Hide Preview Pane
    void hidePreviewPane();

    // Show Hide Preview
    void showHidePreview();

    // Load Recent Files List
    void loadRecentFilesList();
    // Save Recent Files List
    void saveRecentFilesList();
    // Clear Recent Files List
    void clearRecentFilesList();

    // Launch Preferences
    void launchPreferencesDialog();
    // Launch About
    void launchAboutDialog();
    // Launch Help
    void launchHelpDialog();

    // Launch Import Dialog
    void launchImportDialog();
    // Launch Export Dialog
    void launchExportDialog();

    // Load Plugins
    void loadPlugins();
    // Clear Plugins
    void clearPlugins();

    // Update Menu
    void updateMenu();

    // Update Import/Export Submenu
    void updateImportExportSubMenu();

    // Update Recent Files Subment
    void updateRecentFilesSubMenu();

    // Update Window Title
    void updateWindowTitle();

    // Update Translation Language Selector
    void updateTranslationLanguageSelector();

    // Update Translation Font Selector
    void updateTranslationFontSelector();

    // Update Preview Pane
    void updatePreviewPane();

    // Add Recent File
    void addRecentFile(const QString& aPath);

    // Set Checking Active
    void setCheckingActive(const bool& aCheckingActive);

private slots:
    // Current Package Dirty State Changed Slot
    void currentPackageDirtyChanged(const bool& aDirty);

    // Package Import Ready Slot
    void packageImportReady(const QJsonObject& aImportedPackage);
    // Package Export Ready Slot
    void packageExportReady();

    // Package Langauges Changed Slot
    void packageLanguagesChanged(const QStringList& newLanguages);

    // Translation Item About To Be Removed Slot
    void itemAboutToBeRemoved(const int& aIndex);
    // Translation Item Removed Slot
    void itemRemoved(const int& aIndex);

//    // Search Result Item Selected Slot
//    void searchResultItemSelected(const int& aIndex);

    // Action Import Triggered Slot
    void actionImportTriggered();
    // Action Export Triggered Slot
    void actionExportTriggered();

    // Action Recent Triggered Slot
    void actionRecentFileTriggered();

    // Splitter Moved Slot
    void on_mainSplitter_splitterMoved(int aPos, int aIndex);

    // On Action New Package/Project Triggered Slot
    void on_actionNew_triggered();
    // Action Open Triggered Slot
    void on_actionOpen_triggered();
    // Action Save Package Triggered Slot
    void on_actionSave_triggered();
    // Action Save As Triggered Slot
    void on_actionSaveAs_triggered();
    // Action Close Package Triggered Slot
    void on_actionClose_triggered();
    // Action Clear Recent Files Triggered Slot
    void on_actionClearAllRecents_triggered();
    // Action Show Hide Preview Triggered Slot
    void on_actionShowHidePreview_triggered();
    // Action Add Translation Group Triggered Slot
    void on_actionAddGroup_triggered();
    // Action Remove Translation Group Triggered Slot
    void on_actionRemoveGroup_triggered();
    // Action Select All Triggered Slot
    void on_actionSelectAll_triggered();
    // Action Select None Triggered Slot
    void on_actionSelectNone_triggered();
    // Action New Translation Item Triggered Slot
    void on_actionNewTranslationItem_triggered();
    // Action Delete Translation Item Triggered Slot
    void on_actionDeleteTranslationItem_triggered();
    // Action Add Layout Triggered Slot
    void on_actionAddLayout_triggered();
    // Action Remove Layout Triggered Slot
    void on_actionRemoveLayout_triggered();
    // Action Find Items Triggered Slot
    void on_actionFindItems_triggered();
    // Action Filter Triggered Slot
    void on_actionFilter_triggered();
    // Action Project Preferences Slot
    void on_actionProjectPreferences_triggered();
    // Action Check Translation Triggered Slot
    void on_actionCheckTranslation_triggered();
    // Action Check All Translations Triggered Slot
    void on_actionCheckAllTranslations_triggered();
    // Action Preferences Triggered Slot
    void on_actionPreferences_triggered();
    // Action Help Triggered Slot
    void on_actionHelp_triggered();
    // Action About Triggered Slot
    void on_actionAbout_triggered();
    // On Close Preview Button Clicked Slot
    void on_closePreviewButton_clicked();
    // Add Translation Layout Button Clicked Slot
    void on_addLayoutButton_clicked();
    // Previous Translation Layout Button Clicked Slot
    void on_prevLayoutButton_clicked();
    // Next Translation Layout Button clicked Slot
    void on_nextLayoutButton_clicked();
    // Remove Translation Layout Button Clicked Slot
    void on_removeLayoutButton_clicked();

    // Language Combo Box Current Index Changed Slot
    void on_languageComboBox_currentIndexChanged(int index);
    // Max Width Spinner Value Changed Slot
    void on_maxWidthSpinner_valueChanged(int arg1);
    // Max Height Spinner Value Changed Slot
    void on_maxHeightSpinner_valueChanged(int arg1);
    // Max Length Spinner Value Changed Slot
    void on_maxLengthSpinner_valueChanged(int arg1);
    // Clear Max Width Button Clicked Slot
    void on_clearMaxWidthButton_clicked();
    // Clear Max Height Button Clicked Slot
    void on_clearMaxHeightButton_clicked();
    // Clear Max Length Button Clicked Slot
    void on_clearMaxLengthButton_clicked();
    // Max Lines Spinner Value Changed Slot
    void on_maxLinesSpinner_valueChanged(int arg1);
    // Wrap Type Combo Box Current Index Changed Slot
    void on_textWrapComboBox_currentIndexChanged(int index);
    // Elide Type Combo Box Current Index Changed Slot
    void on_elideComboBox_currentIndexChanged(int index);
    // Translation Font Combo Box Current Index Changed Slot
    void on_fontComboBox_currentIndexChanged(int index);
    // Translation Font Size Spinner Value Changed Slot
    void on_fontSizeSpinner_valueChanged(int arg1);
    // Layout Shrinkable Checkbox Clicked Slot
    void on_layoutShrinkableCheckBox_clicked();
    // Alternative Fonts Edit Button Clicked Slot
    void on_altFontsEditButton_clicked();

    // Action Quit Triggered Slot
    void on_actionQuit_triggered();
    // Action Exit Triggered
    void on_actionExit_triggered();

private:
    // UI
    Ui::MainWindow*             ui;

    // Busy State
    bool                        mBusy;

    // Status Message Timer ID
//    int                         mStatusTimerID;

    // Preferences
    Preferences*                mPreferences;

    // Supported Languages
    QStringList                 mSupportedLanguages;

    // Labguage Map
    QMap<QString, QString>      mLanguageMap;

    // Recent Files List
    QStringList                 mRecentFiles;

    // Preferences Dialog
    PreferencesDialog*          mPreferencesDialog;

    // Package Preferences Dialog
    PackagePreferencesDialog*   mPackagePreferencesDialog;

    // About Dialog
    AboutDialog*                mAboutDialog;

    // Info Dialog
    InfoDialog*                 mInfoDialog;

    // Confirmation Dialog
    ConfirmationDialog*         mConfirmDialog;

    // Help Dialog
    HelpDialog*                 mHelpDialog;

    // New Group Dialog
    NewGroupDialog*             mGroupDialog;
    // New Translation Dialog
    NewTranslationDialog*       mTranslationDialog;
    // Alternate Fonts Dialog
    AlternateFontsDialog*       mAlternativeFontsDialog;
    // Find Items Dialog
    FindItemsDialog*            mFindItemsDialog;

    // Current Package
    PackageInfo*                mCurrentPackage;

    // Current Package Path
    QString                     mCurrentPath;

    // Current Group
    PackageGroup*               mCurrentGroup;

    // Current Translation Item
    TranslationItem*            mCurrentItem;

    // Current Translation Item Layout
    TranslationItemLayout*      mCurrentLayout;

    // Last Current Index
    int                         mLastIndex;
    // Current Translation Index in Current Group
    int                         mCurrentIndex;

    // Current Translation Layout Index
    int                         mCurrentLayoutIndex;

    // Preview Pane Was Shown
    bool                        mPreviewWasShown;
    // Last Preview Width
    int                         mLastPreviewWidth;

    // Selected Translation Item Language
    QString                     mSelectedItemLanguage;
    // Preserved Translation Item Language Index
    int                         mPreservedItemLanguage;

    // Import Plugins
    QList<PluginInfo>           mImportPlugins;
    // Export Plugins
    QList<PluginInfo>           mExportPlugins;

    // Cell Edit Mode
    bool                        mCellEditMode;

    // Checking Active State
    bool                        mCheckingActive;
    // Checking Index
    int                         mCheckingIndex;

    // Import Package JSON Object
    QJsonObject                 mImportObject;

    // Loaders Counter
    int                         mLoadersCount;

    // ...
};

#endif // MAINWINDOW_H
