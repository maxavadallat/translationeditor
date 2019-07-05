#include <QDebug>
#include <QQmlEngine>

#include "packagegroup.h"
#include "translationitem.h"
#include "translationitemlayout.h"

#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
PackageGroup::PackageGroup(const QString& aName, PackageInfo* aPackageInfo, QObject* aParent)
    : QAbstractListModel(aParent)
    , mPackage(aPackageInfo)
    , mName(aName)
{
    qDebug() << "PackageGroup created.";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void PackageGroup::init()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // ...
}

//==============================================================================
// Shut Down
//==============================================================================
void PackageGroup::shutDown()
{
    // Clear
    clear();
}

//==============================================================================
// Clear
//==============================================================================
void PackageGroup::clear()
{
    // Iterate Through Items
    while (mItems.count() > 0) {
        // Delete Last Item
        delete mItems.takeLast();
    }
}

//==============================================================================
// Get Group Name
//==============================================================================
QString PackageGroup::groupName()
{
    return mName;
}

//==============================================================================
// Set Group Name
//==============================================================================
void PackageGroup::setGroupName(const QString& aGroupName)
{
    // Check Group Name
    if (mName != aGroupName) {
        // Set Group Name
        mName = aGroupName;
        // Emit Group Name Changed Signal
        emit groupNameChanged(mName);
    }
}

//==============================================================================
// Get Translation Item By Index
//==============================================================================
TranslationItem* PackageGroup::getItem(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < mItems.count()) {
        return mItems[aIndex];
    }

    return nullptr;
}

//==============================================================================
// Find/Get Translation Item By Text ID
//==============================================================================
TranslationItem* PackageGroup::findItem(const QString& aTextID)
{
    // Check Text ID
    if (!aTextID.isEmpty()) {
        // Iterate Through Translations
        for (TranslationItem* item : mItems) {
            // Check Text ID
            if (item->textID() == aTextID) {
                return item;
            }
        }
    }

    return nullptr;
}

//==============================================================================
// Get Item Index
//==============================================================================
int PackageGroup::itemIndex(TranslationItem* aItem)
{
    return mItems.indexOf(aItem);
}

//==============================================================================
// Add Item
//==============================================================================
void PackageGroup::addItem(TranslationItem* aNewItem)
{
    // Check Item
    if (aNewItem && itemIndex(aNewItem) == -1) {
        // Begin Insert Row
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        // Append Item
        mItems << aNewItem;
        // End Insert Rows
        endInsertRows();
    }
}

//==============================================================================
// Remove/Take Item
//==============================================================================
TranslationItem* PackageGroup::removeItem(const int& aIndex, const bool& aDelete)
{
    // Init Translation Item
    TranslationItem* removingItem = nullptr;

    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Begin Remove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);
        // Take Item
        removingItem = mItems.takeAt(aIndex);
        // End Remove Rows
        endRemoveRows();
    }

    // Check Delete Item
    if (aDelete && removingItem) {
        // Delete Item
        delete removingItem;
        // Reset Item Pointer
        removingItem = nullptr;
    }

    return removingItem;
}

//==============================================================================
// Row Count
//==============================================================================
int PackageGroup::rowCount(const QModelIndex& ) const
{
    return mItems.count();
}

//==============================================================================
// Data
//==============================================================================
QVariant PackageGroup::data(const QModelIndex& index, int role) const
{
    // Get Row Index
    int rowIndex = index.row();

    // Check Row Index
    if (rowIndex >= 0 && rowIndex < rowCount()) {
        // Get Translation Item
        TranslationItem* trItem = mItems[rowIndex];

        // Switch Role
        switch (role) {
            case EPIRSourceIndex:   return rowIndex;
            case EPGRTextID:        return trItem->textID();
            case EPGRType:          return trItem->textType();
            case EPGRState:         return trItem->textState();
            case EPGROriginal:      return trItem->originalText();
            case EPGRGroup:         return trItem->translationGroup();
            case EPGRComment:       return trItem->comment();
            case EPGRLanguages:     return trItem->languages();
            case EPGRTranslations:  return trItem->translations();
        }
    }

    return QVariant();
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> PackageGroup::roleNames() const
{
    //qDebug() << "PackageGroup::roleNames";

    // Init Role Names
    QHash<int, QByteArray> rNames;

    rNames[EPIRSourceIndex]     = "triSourceIndex";
    rNames[EPGRTextID]          = "triTextID";
    rNames[EPGRType]            = "triType";
    rNames[EPGRState]           = "triState";
    rNames[EPGROriginal]        = "triOriginal";
    rNames[EPGRGroup]           = "triGroup";
    rNames[EPGRComment]         = "triComment";
    rNames[EPGRLanguages]       = "triLanguages";
    rNames[EPGRTranslations]    = "triTranslations";

    return rNames;
}

//==============================================================================
// Destructor
//==============================================================================
PackageGroup::~PackageGroup()
{
    // Shut Down
    shutDown();

    qDebug() << "PackageGroup deleted.";
}
