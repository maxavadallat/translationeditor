#include <QDebug>

#include "translationitem.h"
#include "packageinfo.h"
#include "packagegroupfilter.h"
#include "preferences.h"

//==============================================================================
// Constructor
//==============================================================================
PackageGroupFilter::PackageGroupFilter(QObject* aParent)
    : QSortFilterProxyModel(aParent)
    , mDescending(false)
    , mSortingColumn(0)
    , mGroupFilters()
    , mSearchFilter("")
    , mShowChecked(true)
    , mShowDraft(true)
    , mShowWarning(true)
    , mPreferences(Preferences::getInstance())
{
    qDebug() << "PackageGroupFilter created.";

    // Clear Group Filters
    mGroupFilters.clear();

    // Set Sorting Column
    mSortingColumn = mPreferences->sortingColumn();
    // Set Sorting Order
    mDescending = mPreferences->sortingOrder();

    // Set Sorting Role
    setSortRole(PackageInfo::EPIRSourceIndex + mSortingColumn);
    // Sort
    sort(0, mDescending ? Qt::DescendingOrder : Qt::AscendingOrder);

//    connect(this, &QAbstractProxyModel::sourceModelChanged, this, &PackageGroupFilter::sourceModelSet);
//    connect(this, &QAbstractItemModel::modelReset, this, &PackageGroupFilter::modelReset);
//    connect(this, &QAbstractListModel::rowsInserted, this, &PackageGroupFilter::modelRowsInserted);

//    // Set Dynamic Sort Filter
//    setDynamicSortFilter(true);

    // ...
}

//==============================================================================
// Get Descending Order
//==============================================================================
bool PackageGroupFilter::descending()
{
    return mDescending;
}

//==============================================================================
// Set Descending Order
//==============================================================================
void PackageGroupFilter::setDescending(const bool& aDescending)
{
    // Check Descending
    if (mDescending != aDescending) {
        qDebug() << "PackageGroupFilter::setDescending - aDescending: " << aDescending;

        // Set Descending Order
        mDescending = aDescending;
        // Emit Descending Order Changed Signal
        emit descendingChanged(mDescending);

        // Store Preference
        mPreferences->setSortingOrder(mDescending);

        // Sort
        sort(0, mDescending ? Qt::DescendingOrder : Qt::AscendingOrder);
    }
}

//==============================================================================
// Get Sorting Column
//==============================================================================
int PackageGroupFilter::sortingColumn()
{
    return mSortingColumn;
}

//==============================================================================
// Set Sorting Column
//==============================================================================
void PackageGroupFilter::setSortingColumn(const int& aColumn)
{
    // Chekc Sorting Column
    if (mSortingColumn != aColumn) {
        qDebug() << "PackageGroupFilter::setSortingColumn - aColumn: " << aColumn;

        // Set Sorting Column
        mSortingColumn = aColumn;
        // Emit Sorting Column Changed Signal
        emit sortingColumnChanged(mSortingColumn);

        // Store Preference
        mPreferences->setSortingColumn(mSortingColumn);

        // Set Sorting Role
        setSortRole(PackageInfo::EPIRSourceIndex + mSortingColumn);

        // Check Descending Order
        if (mDescending) {
            // Set Descending Order
            setDescending(false);
        } else {
            // Sort
            sort(0, mDescending ? Qt::DescendingOrder : Qt::AscendingOrder);
        }
    }
}

//==============================================================================
// Get Group Filter
//==============================================================================
QStringList PackageGroupFilter::groupFilters()
{
    return mGroupFilters;
}

//==============================================================================
// Set Group Filters
//==============================================================================
void PackageGroupFilter::setGroupFilters(const QStringList& aGroupFilters)
{
    // Check Group Filters
    if (mGroupFilters != aGroupFilters) {
        qDebug() << "PackageGroupFilter::setGroupFilters - aGroupFilters: " << aGroupFilters;

        // Set Group Filters
        mGroupFilters = aGroupFilters;
        // Emit Group Filters Changed Signal
        emit groupFilterChanged(mGroupFilters);

        // Invalidate Filter
        invalidateFilter();
    }
}

//==============================================================================
// Get Search Filter
//==============================================================================
QString PackageGroupFilter::searchFilter()
{
    return mSearchFilter;
}

//==============================================================================
// Set Search Filter
//==============================================================================
void PackageGroupFilter::setSearchFilter(const QString& aSearchFilter)
{
    // Check Search Filter
    if (mSearchFilter != aSearchFilter) {
        // Set Search Filter
        mSearchFilter = aSearchFilter;
        // Emit Search Filter Changed Signal
        emit searchFilterChanged(mSearchFilter);

        // Invalidate Filter
        invalidateFilter();
    }
}

//==============================================================================
// Get Show Checked
//==============================================================================
bool PackageGroupFilter::showChecked()
{
    return mShowChecked;
}

//==============================================================================
// Set Show Checked
//==============================================================================
void PackageGroupFilter::setShowChecked(const bool& aShowChecked)
{
    // Check Show Checked
    if (mShowChecked != aShowChecked) {
        // Set Show Checked
        mShowChecked = aShowChecked;
        // Emit Show Checked Changed Signal
        emit showCheckedChanged(mShowChecked);

        // Invalidate Filter
        invalidateFilter();
    }
}

//==============================================================================
// Get Show Draft
//==============================================================================
bool PackageGroupFilter::showDraft()
{
    return mShowDraft;
}

//==============================================================================
// SetShow Draft
//==============================================================================
void PackageGroupFilter::setShowDraft(const bool& aShowDraft)
{
    // Check Show Draft
    if (mShowDraft != aShowDraft) {
        // Set Show Draft
        mShowDraft = aShowDraft;
        // Emit Show Draft Changed Signal
        emit showDraftChanged(mShowDraft);

        // Invalidate Filter
        invalidateFilter();
    }
}

//==============================================================================
// Get Show Warning
//==============================================================================
bool PackageGroupFilter::showWarning()
{
    return mShowWarning;
}

//==============================================================================
// Set Show Warning
//==============================================================================
void PackageGroupFilter::setShowWarning(const bool& aShowWarning)
{
    // Check Show Warning
    if (mShowWarning != aShowWarning) {
        // Set Show Warning
        mShowWarning = aShowWarning;
        // Emit Show Warning Changed Signal
        emit showWarningChanged(mShowWarning);

        // Invalidate Filter
        invalidateFilter();
    }
}


//==============================================================================
// Get Count
//==============================================================================
int PackageGroupFilter::count()
{
    return rowCount();
}

//==============================================================================
// Add Group Filter
//==============================================================================
void PackageGroupFilter::addGroupFilter(const QString& aGroupName)
{
    // Check Group Name
    if (!aGroupName.isEmpty()) {
        // Get Index
        int gIndex = mGroupFilters.indexOf(aGroupName);
        // Check Index
        if (gIndex == -1) {
            qDebug() << "PackageGroupFilter::addGroupFilter - aGroupName: " << aGroupName;

            // Append Group
            mGroupFilters << aGroupName;
            // Sort
            mGroupFilters.sort();

            // Emit Group Filters Changed Signal
            emit groupFilterChanged(mGroupFilters);

            // Invalidate Filter
            invalidateFilter();
        }
    }
}

//==============================================================================
// Remove Group Filter
//==============================================================================
void PackageGroupFilter::removeGroupFilter(const QString& aGroupName)
{
    // Check Group Name
    if (!aGroupName.isEmpty()) {
        // Get Index
        int gIndex = mGroupFilters.indexOf(aGroupName);
        // Check Index
        if (gIndex != -1) {
            qDebug() << "PackageGroupFilter::removeGroupFilter - aGroupName: " << aGroupName;

            // Remove Group
            mGroupFilters.removeAt(gIndex);

            // Emit Group Filters Changed Signal
            emit groupFilterChanged(mGroupFilters);

            // Invalidate Filter
            invalidateFilter();
        }
    }
}

//==============================================================================
// Map Index To Source
//==============================================================================
int PackageGroupFilter::mapIndexToSource(int proxyIndex) const
{
    return mapToSource(index(proxyIndex, 0)).row();
}

//==============================================================================
// Map Index From Source
//==============================================================================
int PackageGroupFilter::mapIndexFromSource(int sourceIndex) const
{
    return mapFromSource(sourceModel()->index(sourceIndex, 0)).row();
}

//==============================================================================
// Less Tnan - Sorting Condition
//==============================================================================
bool PackageGroupFilter::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
    //qDebug() << "PackageGroupFilter::lessThan - leftRow: " << source_left.row() << " - rightRow: " << source_right.row();

    // Check Sorting Column
    if (mSortingColumn == 0) {
        // Get Left Source Index
        int leftIndex = sourceModel()->data(source_left, PackageInfo::EPIRSourceIndex).toInt();
        // Get Right Source Index
        int rightIndex = sourceModel()->data(source_right, PackageInfo::EPIRSourceIndex).toInt();

        return leftIndex < rightIndex;
    }

    // Get Left Group
    QString leftGroup = sourceModel()->data(source_left, PackageInfo::EPIRGroup).toString();
    // Get Right Group
    QString rightGroup = sourceModel()->data(source_right, PackageInfo::EPIRGroup).toString();

    // Compare Groups
    if (leftGroup != rightGroup) {
        return mDescending ? (leftGroup > rightGroup) : (leftGroup < rightGroup);
    }

    // Get Left Column String
    QString leftColumn = sourceModel()->data(source_left, PackageInfo::EPIRSourceIndex + mSortingColumn).toString();
    // Get Right Column String
    QString rightColumn = sourceModel()->data(source_right, PackageInfo::EPIRSourceIndex + mSortingColumn).toString();

    //qDebug() << "PackageGroupFilter::lessThan - leftColumn: " << leftColumn << " - rightColumn: " << rightColumn;

    return leftColumn < rightColumn;
}

//==============================================================================
// Filter Accept Row - Show Only Filtered Items
//==============================================================================
bool PackageGroupFilter::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    // Get Group Index
    QModelIndex groupIndex = sourceModel()->index(source_row, 0, source_parent);

    // Get Item State
    QString itemState = sourceModel()->data(groupIndex, PackageInfo::EPIRState).toString();

    // Check Filters - Checked
    if (!mShowChecked && itemState == TEXT_STATE_CHECKED) {
        return false;
    }

    // Check Filters - Draft
    if (!mShowDraft && itemState == TEXT_STATE_DRAFT) {
        return false;
    }

    // Check Filters - Warning
    if (!mShowWarning && itemState == TEXT_STATE_WARNING) {
        return false;
    }

    // Get Item Group Name
    QString groupName = sourceModel()->data(groupIndex, PackageInfo::EPIRGroup).toString();

    //qDebug() << "PackageGroupFilter::filterAcceptsRow - source_row: " << source_row << " - groupName: " << groupName;

    // Get Index
    int gIndex = mGroupFilters.indexOf(groupName);

    // Check Group Index
    if (gIndex != -1 && !mSearchFilter.isEmpty()) {
        //qDebug() << "PackageGroupFilter::filterAcceptsRow - mSearchFilter: " << mSearchFilter;

        // Get Source
        PackageInfo* source = dynamic_cast<PackageInfo*>(sourceModel());

        return source ? source->acceptRow(source_row, mSearchFilter) : false;
    }

    return (gIndex != -1);
}

//==============================================================================
// Get Source Model Languages
//==============================================================================
QStringList PackageGroupFilter::languages() const
{
    // Get Package Info
    PackageInfo* packageInfoSourceModel = dynamic_cast<PackageInfo*>(sourceModel());

    // Check Package Info
    if (packageInfoSourceModel) {
        return packageInfoSourceModel->languages();
    }

    return QStringList();
}

//==============================================================================
// Source Model Set Slot
//==============================================================================
void PackageGroupFilter::sourceModelSet()
{
    qDebug() << "PackageGroupFilter::sourceModelSet";

    // ...

    // Sort
    sort(0, mDescending ? Qt::DescendingOrder : Qt::AscendingOrder);
}

//==============================================================================
// Model Reset Slot
//==============================================================================
void PackageGroupFilter::modelReset()
{
    qDebug() << "PackageGroupFilter::modelReset";

    // ...
}

//==============================================================================
// Model Rows Inserted
//==============================================================================
void PackageGroupFilter::modelRowsInserted()
{
    qDebug() << "PackageGroupFilter::modelRowsInserted";

    // ...

    // Invalidate Filter
    invalidateFilter();

    // Sort
    //sort(0, mDescending ? Qt::DescendingOrder : Qt::AscendingOrder);
}

//==============================================================================
// Update Roles
//==============================================================================
void PackageGroupFilter::updateRoles()
{
    qDebug() << "PackageGroupFilter::updateRoles";

//    // Set Filter Role
//    setFilterRole(PackageInfo::EPIRTextID + mSortingColumn - 1);
    // Set Sort Role
    setSortRole(PackageInfo::EPIRTextID + mSortingColumn - 1);

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
PackageGroupFilter::~PackageGroupFilter()
{
    // Clear Group Filters
    mGroupFilters.clear();

    // Check Preferences
    if (mPreferences) {
        // Release Preferences Instance
        mPreferences->release();
    }

    // ...

    qDebug() << "PackageGroupFilter deleted.";
}
