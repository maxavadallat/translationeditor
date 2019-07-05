#ifndef PACKAGEGROUP_H
#define PACKAGEGROUP_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>

class PackageInfo;
class TranslationItem;

//==============================================================================
// Package Group Class
//==============================================================================
class PackageGroup : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)

public:
    explicit PackageGroup(const QString& aName, PackageInfo* aPackageInfo, QObject* aParent = nullptr);

    // Get Group Name
    QString groupName();
    // Set Group Name
    void setGroupName(const QString& aGroupName);

    // Get Translation Item By Index
    TranslationItem* getItem(const int& aIndex);
    // Find/Get Translation Item By Text ID
    TranslationItem* findItem(const QString& aTextID);

    // Get Item Index
    int itemIndex(TranslationItem* aItem);

    // Add Item
    void addItem(TranslationItem* aNewItem);
    // Remove/Take Item
    TranslationItem* removeItem(const int& aIndex, const bool& aDelete = true);

    // Destructor
    ~PackageGroup();

signals:
    // Group Name Changed Signal
    void groupNameChanged(const QString& newGroupName);

public: // from QAbstractListModel
    // Row Count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    // Data
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    // Get Role Names
    virtual QHash<int, QByteArray> roleNames() const;

protected:
    // Init
    void init();
    // Shut Down
    void shutDown();
    // Clear
    void clear();

protected:
    friend class MainWindow;
    friend class PackageInfo;

    // Item Field Roles
    enum EPGItemRoles {
        EPIRSourceIndex = Qt::UserRole + 1,
        EPGRTextID,
        EPGRType,
        EPGRState,
        EPGROriginal,
        EPGRGroup,
        EPGRComment,
        EPGRLanguages,
        EPIRMergedTrs,
        EPGRTranslations
    };

    // Package Info
    PackageInfo*                mPackage;
    // Group Name
    QString                     mName;
    // Items
    QList<TranslationItem*>     mItems;

    // ...

};

#endif // PACKAGEGROUP_H
