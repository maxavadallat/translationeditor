#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QAbstractListModel>

class PackageGroup;
class TranslationItem;
class PackageGroupFilter;

//==============================================================================
// Package Info Class
//==============================================================================
class PackageInfo : public QAbstractListModel
{
    Q_OBJECT

    // Package Name
    Q_PROPERTY(QString packageName READ packageName WRITE setPackageName NOTIFY packageNameChanged)
    // Fonts Dir
    Q_PROPERTY(QString fontsDir READ fontsDir WRITE setFontsDir NOTIFY fontsDirChanged)
    // Default Font
    Q_PROPERTY(QString defaultFont READ defaultFont WRITE setDefaultFont NOTIFY defaultFontChanged)
    // Default Font Size
    Q_PROPERTY(int defaultFontSize READ defaultFontSize WRITE setDefaultFontSize NOTIFY defaultFontSizeChanged)
    // Available Languages
    Q_PROPERTY(QStringList languages READ languages NOTIFY languagesChanged)
    // Groups
    Q_PROPERTY(QStringList groups READ groups NOTIFY groupsChanged)

    // Selected Groups for Showing
    Q_PROPERTY(QStringList selectedGroups READ selectedGroups NOTIFY selectedGroupsChanged)
    // Selected Languages for Showing
    Q_PROPERTY(QStringList selectedLanguages READ selectedLanguages NOTIFY selectedLanguagesChanged)

    // All Languages Selected
    Q_PROPERTY(bool allLanguages READ allLanguages NOTIFY allLanguagesChanged)

    // Current Group
    Q_PROPERTY(PackageGroup* currentGroup READ currentGroup NOTIFY currentGroupChanged)

    // Filter Text
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)

    // Is Dirty
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY dirtyChanged)

    // Selected Item Count
    Q_PROPERTY(int selectedItemCount READ selectedItemCount NOTIFY selectedItemCountChanged)

    // Single (Popup) Selection Index
    Q_PROPERTY(int singleSelectionIndex READ singleSelectionIndex WRITE setSingleSelectionIndex NOTIFY singleSelectionIndexChanged)

    // Count
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    // From JSON Object
    static PackageInfo* fromJSON(const QJsonObject& aObject);

    // Constructor
    explicit PackageInfo(QObject* aParent = nullptr);

    // Get Path
    QString path();

    // Load Package
    void loadPackage(const QString& aPackagePath);
    // Save Package
    void savePackage(const QString& aPackagePath = "");

    // Add Language
    Q_INVOKABLE void addLanguage(const QString& aLanguage);
    // Remove Language
    Q_INVOKABLE void removeLanguage(const QString& aLanguage);

    // Add Group
    Q_INVOKABLE void addGroup(const QString& aGroupName);
    // Remove Group
    Q_INVOKABLE void removeGroup(const QString& aGroupName);

    // Select Group
    void selectGroup(const QString& aGroupName);

    // Add Translation Item
    bool addTranslationItem(const QString& aGroupName, TranslationItem* aTranslationItem);
    // Remove Translation Item
    bool removeTranslationItem(const QString& aGroupName, const int& aIndex);

    // Remove Translation Item By Index
    Q_INVOKABLE void removeTranslationItem(const int& aIndex = -1);

    // Get Package Name
    QString packageName();
    // Set Package Name
    void setPackageName(const QString& aName);

    // Get Fonts Dir
    QString fontsDir();
    // Set Fonts Dir
    void setFontsDir(const QString& aFontsDir);

    // Get Default Font Name
    QString defaultFont();
    // Set Default Font Name
    void setDefaultFont(const QString& aFontName);

    // Get Default Font Size
    int defaultFontSize();
    // Set Default Font Size
    void setDefaultFontSize(const int& aFontSize);

    // Get Groups
    QStringList groups();
    // Get Available Languages
    QStringList languages();

    // Get Selected Groups
    QStringList selectedGroups();
    // Get Selected Languages
    QStringList selectedLanguages();

    // All Languages Selected
    bool allLanguages();

    // Get Current Group
    PackageGroup* currentGroup();

    // Get Filter Text
    QString filterText();
    // Set Filter Text
    void setFilterText(const QString& aFilterText);

    // Get Dirty State
    bool isDirty();

    // Get Selected Item Count
    int selectedItemCount();

    // Get Single (Popup) Selection Index
    int singleSelectionIndex();
    // Set Single (Popup) Selection Index
    void setSingleSelectionIndex(const int& aSelectionIndex);

    // Get Count
    int count();

    // Get Package Group By Name
    PackageGroup* packageGroup(const QString& aName);
    // Get Package Group By Index
    Q_INVOKABLE PackageGroup* packageGroup(const int& aIndex);
    // Select All Groups
    Q_INVOKABLE void selectAllGroups();
    // Deselect All Groups
    Q_INVOKABLE void deselectAllGroups();
    // Set Selected Group
    Q_INVOKABLE void setSelectedGroup(const QString& aGroupName, const bool& aSelected);
    // Set Selected Language
    Q_INVOKABLE void setSelectedLanguage(const QString& aLanguage, const bool& aSelected);

    // Select All Languages
    Q_INVOKABLE void setAllLanguagesSelected(const bool& aSelected);

    // Get Translation Item By Index
    Q_INVOKABLE TranslationItem* getItem(const int& aIndex) const;

    // Set Item Selected
    Q_INVOKABLE void setItemSelected(const int& aIndex, const bool& aSelected);

    // Select All Items
    Q_INVOKABLE void selectAllItems();
    // Select No Items
    Q_INVOKABLE void selectNoItems();

    // Move Item(s) To a Different Group, If Index is -1 Moves all Selected Items
    Q_INVOKABLE void moveItem(const QString& aGroup, const int& aIndex = -1);

    // Find/Get Translation Item By Text ID
    TranslationItem* findItem(const QString& aTextID);

    // Get Translation Item Index
    int itemIndex(TranslationItem* aItem);

    // Refresh Item
    Q_INVOKABLE void refresh(const int& aIndex);

    // Accept Row
    bool acceptRow(const int& aIndex, const QString& aFilterText) const;

    // Accept Item
    bool acceptItem(TranslationItem* aItem, const QString& aFilterText) const;

    // Export To JSON Object
    QJsonObject exportToJSON();

    // ...

    // Destructor
    ~PackageInfo();

signals:
    // Package Name Changed Signal
    void packageNameChanged(const QString& newName);
    // Fonst Dir Changed Signal
    void fontsDirChanged(const QString& newFontsDir);
    // Default Font Changed Signal
    void defaultFontChanged(const QString& newDefaultFont);
    // Default Font Size Changd Signal
    void defaultFontSizeChanged(const int& newDefaultFontSize);
    // Groups Changed Signal
    void groupsChanged(const QStringList& newGroups);
    // Available Langauges Changed Signal
    void languagesChanged(const QStringList& newLanguages);
    // Selected Groups Changed Signal
    void selectedGroupsChanged(const QStringList& newSelectedGroups);
    // Selected Langauges Changed Signal
    void selectedLanguagesChanged(const QStringList& newSelectedLanguages);
    // All Selected Languages Changed Signal
    void allLanguagesChanged(const bool& aAllSelected);
    // Current group Changed Signal
    void currentGroupChanged(PackageGroup* newCurrentGroup);
    // Filter Text Changed Signal
    void filterTextChanged(const QString& newFilterText);
    // Dirty State changed Signal
    void dirtyChanged(const bool& aDirty);
    // Selected Item Count Changed Signal
    void selectedItemCountChanged(const int& newSelectedCount);
    // Single Selection Index Changd Signal
    void singleSelectionIndexChanged(const int& aSelectionIndex);

    // Need Remove Language Confirmation
    void needLanguageRemovalConfirmation(const QString& aLanguage);

    // Item About to be Removed Signal
    void itemAboutToBeRemoved(const int& aIndex);
    // Item Removed Signal
    void itemRemoved(const int& aIndex);

    // Count Changed Signal
    void countChanged(const int& newCount);

    // ...

public: // from QAbstractListModel
    // Row Count
    Q_INVOKABLE virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    // Data
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    // Get Role Names
    virtual QHash<int, QByteArray> roleNames() const;

protected:
    friend class TranslationItem;
    friend class TranslationItemLayout;

    // Init
    void init();
    // Shut Down
    void shutDown();
    // Clear
    void clear();

    // Set Dirty State
    void setDirty(const bool& aDirty = true);

    // Import From JSON Object
    void importFromJSON(const QJsonObject& aObject);

    // ...

public:
    // Item Field Roles
    enum EPIItemRoles {
        EPIRSourceIndex = Qt::UserRole + 1,
        EPIRTextID,
        EPIRScreen,
        EPIRState,
        EPIROriginal,
        EPIRType,
        EPIRGroup,
        EPIRComment,
        EPIRSelected,
        EPIRLanguages,
        EPIRMergedTrs,
        EPIRTranslations
    };

private:
    // Package File Path
    QString                         mFilePath;

    // Package Name
    QString                         mName;

    // Fonst Dir
    QString                         mFontsDir;

    // Default Font
    QString                         mDefaultFont;

    // Default Font Size
    int                             mDefaultFontSize;

    // Languages
    QStringList                     mLanguages;

    // Selected Languages
    QStringList                     mSelectedLanguages;

    // All Languages Selected
    bool                            mAllLanguages;

    // Groups
    QMap<QString, PackageGroup*>    mGroups;

    // Selected Groups
    QStringList                     mSelectedGroups;

    // Current Group
    PackageGroup*                   mCurrentGroup;

    // Filter Text
    QString                         mFilterText;

    // Dirty State
    bool                            mDirty;

    // Selected Item Count
    int                             mSelectedItemCount;

    // Single Selection Index
    int                             mSingleSelectionIndex;
};

#endif // PACKAGEINFO_H
