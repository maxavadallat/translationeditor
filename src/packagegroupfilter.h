#ifndef PACKAGEGROUPFILTER_H
#define PACKAGEGROUPFILTER_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QStringList>

class Preferences;

//==============================================================================
// Package Group Filter Class
//==============================================================================
class PackageGroupFilter : public QSortFilterProxyModel
{
    Q_OBJECT

    // Descending Order
    Q_PROPERTY(bool descending READ descending WRITE setDescending NOTIFY descendingChanged)
    // Sorting Column
    Q_PROPERTY(int sortingColumn READ sortingColumn WRITE setSortingColumn NOTIFY sortingColumnChanged)
    // Group Filter
    Q_PROPERTY(QStringList groupFilters READ groupFilters NOTIFY groupFilterChanged)

    // Search Filter
    Q_PROPERTY(QString searchFilter READ searchFilter WRITE setSearchFilter NOTIFY searchFilterChanged)

    // Show Checked
    Q_PROPERTY(bool showChecked READ showChecked WRITE setShowChecked NOTIFY showCheckedChanged)
    // Show Draft
    Q_PROPERTY(bool showDraft READ showDraft WRITE setShowDraft NOTIFY showDraftChanged)
    // Show Warning
    Q_PROPERTY(bool showWarning READ showWarning WRITE setShowWarning NOTIFY showWarningChanged)

    // Count
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    // Constructor
    explicit PackageGroupFilter(QObject* aParent = nullptr);

    // Get Descending Order
    bool descending();
    // Set Descending Order
    void setDescending(const bool& aDescending);

    // Get Sorting Column
    int sortingColumn();
    // Set Sorting Column
    void setSortingColumn(const int& aColumn);

    // Get Group Filter
    QStringList groupFilters();
    // Set Group Filters
    Q_INVOKABLE void setGroupFilters(const QStringList& aGroupFilters);

    // Get Search Filter
    QString searchFilter();
    // Set Search Filter
    void setSearchFilter(const QString& aSearchFilter);

    // Get Show Checked
    bool showChecked();
    // Set Show Checked
    void setShowChecked(const bool& aShowChecked);

    // Get Show Draft
    bool showDraft();
    // SetShow Draft
    void setShowDraft(const bool& aShowDraft);

    // Get Show Warning
    bool showWarning();
    // Set Show Warning
    void setShowWarning(const bool& aShowWarning);

    // Get Count
    int count();

    // Add Group Filter
    Q_INVOKABLE void addGroupFilter(const QString& aGroupName);
    // Remove Group Filter
    Q_INVOKABLE void removeGroupFilter(const QString& aGroupName);

    // Map Index To Source
    Q_INVOKABLE int mapIndexToSource(int proxyIndex) const;
    // Map Index From Source
    Q_INVOKABLE int mapIndexFromSource(int sourceIndex) const;

    // Destructor
    ~PackageGroupFilter();

signals:
    // Descending Order Changed Signal
    void descendingChanged(const bool& aDescending);
    // Sorting Column Changed Signal
    void sortingColumnChanged(const int& newColumn);
    // Group Filters Changed Signal
    void groupFilterChanged(const QStringList& newGroupFilters);
    // Search Filter Changed Signal
    void searchFilterChanged(const QString& newSearchFilter);

    // Show Checked Changed Signal
    void showCheckedChanged(const bool& aShowChecked);
    // Show Draft Changed Signal
    void showDraftChanged(const bool& aShowDraft);
    // Show Warning Changed Signal
    void showWarningChanged(const bool& aShowWarning);

    // Count Changed Signal
    void countChanged();

protected: // from QSortFilterProxyModel
    // Less Tnan - Sorting Condition
    virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const;
    // Filter Accept Row - Show Only Filtered Items
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

protected:
    // Get Source Model Languages
    QStringList languages() const;

protected slots:
    // Source Model Set Slot
    void sourceModelSet();
    // Model Reset Slot
    void modelReset();
    // Model Rows Inserted
    void modelRowsInserted();

    // Update Roles
    void updateRoles();

protected: // Data
    // Order
    bool            mDescending;
    // Sorting Column Index
    int             mSortingColumn;
    // Group Filters
    QStringList     mGroupFilters;
    // Search Filter
    QString         mSearchFilter;

    // Show Checked
    bool            mShowChecked;
    // Show Draft
    bool            mShowDraft;
    // Show Warning
    bool            mShowWarning;

    // Preferences
    Preferences*    mPreferences;
};

#endif // PACKAGEGROUPFILTER_H
