#ifndef FINDITEMSRESULTMODEL_H
#define FINDITEMSRESULTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QThread>

class PackageInfo;
class TranslationItem;
class ResultItem;
class FindItemsWorker;

//==============================================================================
// Find Items Results Model Class
//==============================================================================
class FindItemsResultModel : public QAbstractListModel
{
    Q_OBJECT

    // Busy - Search Active
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    // Search Term
    Q_PROPERTY(QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged)
    // Case Sensitive
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY caseSensitiveChanged)
    // Whole Words
    Q_PROPERTY(bool wholeWords READ wholeWords WRITE setWholeWords NOTIFY wholeWordsChanged)

public:
    // Constructor
    explicit FindItemsResultModel(PackageInfo* aPackage, QObject* aParent = nullptr);

    // Set Package
    void setPackage(PackageInfo* aPackage);

    // Get Busy State
    bool busy();

    // Get Search Term
    QString searchTerm();
    // Set Search Term
    void setSearchTerm(const QString& aSearchTerm);

    // Get Case Sensitive
    bool caseSensitive();
    // Set Case Sensitive
    void setCaseSensitive(const bool& aCaseSensitive);

    // Get Whole Words
    bool wholeWords();
    // Set Whole Words
    void setWholeWords(const bool& aWholeWords);

    // Start Search
    void startSearch();
    // Stop Search
    void stopSearch();

    // Destructor
    ~FindItemsResultModel();

signals:
    // Busy State Changed Signal
    void busyChanged(const bool& aBusy);
    // Search Term Changed Signal
    void searchTermChanged(const QString& aSearchTerm);
    // Case Sensitive Changed Signal
    void caseSensitiveChanged(const bool& aCaseSensitive);
    // Whole Words Changed Signal
    void wholeWordsChanged(const bool& aWholeWords);

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

    // Start Worker
    void startWorker();
    // Stop Worker
    void stopWorker();
    // Restart Worker
    void restartWorker();

protected slots:
    // Worker Started Slot
    void workerStarted();
    // Worker Aborted
    void workerAborted();
    // Worker Finished Slot
    void workerFinished();
    // Add Result Item Slot
    void addResultItem(const int& aIndex, const QString& aLabelID, const QString& aOriginalText, const QString& aMatches);

protected: // Data
    friend class FindItemsWorker;

    // Item Field Roles
    enum EFIRItemRoles {
        EFIRSourceIndex = Qt::UserRole + 1,
        EFIRLabelID,
        EFIROriginalText,
        EFIRMatches
    };

    // Package Info
    PackageInfo*                mPackage;

    // Search Term
    QString                     mSearchTerm;
    // Case Sensitive
    bool                        mCaseSensitive;
    // Whole Words
    bool                        mWholeWords;

    // Results
    QList<ResultItem*>          mResults;

    // Worker
    FindItemsWorker*            mWorker;
};








//==============================================================================
// Find Items Result Item Class
//==============================================================================
class ResultItem : public QObject
{
    Q_OBJECT

public:
    explicit ResultItem(QObject* aParent = nullptr) : QObject(aParent) { }

    // Source Index
    int         mIndex;
    // Label ID
    QString     mLabelID;
    // Original Text
    QString     mOriginalText;
    // Matches
    QString     mMatches;
};







//==============================================================================
// Find Items Worker Class
//==============================================================================
class FindItemsWorker : public QThread
{
    Q_OBJECT

public:
    // Constructor
    explicit FindItemsWorker(FindItemsResultModel* aResultModel, QObject* aParent = nullptr);

    // Stop
    void stop();

    // Destructor
    ~FindItemsWorker();

public: // From QThread
    // Run
    virtual void run();

signals:
    // Search Started Signal
    void searchStarted();
    // Search Aborted Signal
    void searchAborted();
    // Search Finished Signal
    void searchFinished();
    // Add Result Item Signal
    void addResultItem(const int& aIndex, const QString& aLabelID, const QString& aOriginalText, const QString& aMatches);

protected:
    // Check Item For Matches
    QString checkItem(TranslationItem* aItem);

protected:
    friend class FindItemsResultModel;

    // Result Model
    FindItemsResultModel*   mResultModel;
    // Search Term
    QString                 mSearchTerm;
    // Case Sensitive Match
    bool                    mCaseSensitive;
    // Whole Words Match
    bool                    mWholeWords;
    // Search Running
    bool                    mSearchRunning;
};






#endif // FINDITEMSRESULTMODEL_H
