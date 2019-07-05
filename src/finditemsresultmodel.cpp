#include <QQmlEngine>
#include <QApplication>
#include <QDebug>

#include "packageinfo.h"
#include "translationitem.h"
#include "finditemsresultmodel.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
FindItemsResultModel::FindItemsResultModel(PackageInfo* aPackage, QObject* aParent)
    : QAbstractListModel(aParent)
    , mPackage(aPackage)
    , mSearchTerm("")
    , mCaseSensitive(false)
    , mWholeWords(false)
    , mWorker(nullptr)
{
    // Set Ownership
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void FindItemsResultModel::init()
{
    qDebug() << "FindItemsResultModel::init";

    // Check Worker
    if (!mWorker) {
        // Create Worker
        mWorker = new FindItemsWorker(this);

        // Connect Signals
        connect(mWorker, &FindItemsWorker::searchStarted, this, &FindItemsResultModel::workerStarted);
        connect(mWorker, &FindItemsWorker::searchAborted, this, &FindItemsResultModel::workerAborted);
        connect(mWorker, &FindItemsWorker::searchFinished, this, &FindItemsResultModel::workerFinished);

        connect(mWorker, &FindItemsWorker::addResultItem, this, &FindItemsResultModel::addResultItem);
    }

    // ...
}

//==============================================================================
// Shut Down
//==============================================================================
void FindItemsResultModel::shutDown()
{
    qDebug() << "FindItemsResultModel::shutDown";

    // Clear
    clear();

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void FindItemsResultModel::clear()
{
    qDebug() << "FindItemsResultModel::clear";

    // Begin REset Model
    beginResetModel();

    // Iterate Through Results
    while (mResults.count() > 0) {
        delete  mResults.takeLast();
    }

    // End Reset Model
    endResetModel();
}

//==============================================================================
// Start Worker
//==============================================================================
void FindItemsResultModel::startWorker()
{
    // Check Worker
    if (mWorker) {
        qDebug() << "FindItemsResultModel::startWorker";

        // Start Worker
        mWorker->start(QThread::LowestPriority);
    }
}

//==============================================================================
// Stop Worker
//==============================================================================
void FindItemsResultModel::stopWorker()
{
    // Check Worker
    if (mWorker && mWorker->isRunning()) {
        qDebug() << "FindItemsResultModel::stopWorker";

        // Stop Worker
        mWorker->stop();
    }
}

//==============================================================================
// Restart Worker
//==============================================================================
void FindItemsResultModel::restartWorker()
{
    // Check Worker
    if (mWorker) {
        qDebug() << "FindItemsResultModel::restartWorker";

        // Check If Running
        if (mWorker->isRunning()) {
            // Stop
            mWorker->stop();
        }
        // Start Worker
        mWorker->start(QThread::LowestPriority);
    }
}

//==============================================================================
// Worker Started Slot
//==============================================================================
void FindItemsResultModel::workerStarted()
{
    qDebug() << "FindItemsResultModel::workerStarted";

    // Emit Busy Changed Signal
    emit busyChanged(busy());

    // ...
}

//==============================================================================
// Worker Aborted
//==============================================================================
void FindItemsResultModel::workerAborted()
{
    qDebug() << "FindItemsResultModel::workerAborted";

    // Emit Busy Changed Signal
    emit busyChanged(busy());

    // ...

}

//==============================================================================
// Worker Finished Slot
//==============================================================================
void FindItemsResultModel::workerFinished()
{
    qDebug() << "FindItemsResultModel::workerFinished";

    // Emit Busy Changed Signal
    emit busyChanged(busy());

    // ...

}

//==============================================================================
// Add Result Item
//==============================================================================
void FindItemsResultModel::addResultItem(const int& aIndex, const QString& aLabelID, const QString& aOriginalText, const QString& aMatches)
{
    // Check Package
    if (!mPackage) {
        return;
    }

    // Check Index
    if (aIndex >= 0 && aIndex < mPackage->count()) {
        qDebug() << "FindItemsResultModel::addResultItem - aIndex: " << aIndex << " - aLabelID: " << aLabelID << " - aOriginalText: " << aOriginalText;

        // Create New Result Item
        ResultItem* newItem = new ResultItem();

//        // Check Thread
//        if (qApp->thread() != newItem->thread()) {
//            // Move To Main Application Thread
//            newItem->moveToThread(qApp->thread());
//        }

        // Set Up Result Item
        newItem->mIndex = aIndex;
        newItem->mLabelID = aLabelID;
        newItem->mOriginalText = aOriginalText;
        newItem->mMatches = aMatches;

        // Begin Insert Row
        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        // Append New Result Item
        mResults << newItem;

        // End Insert Rows
        endInsertRows();
    }
}

//==============================================================================
// Set Package
//==============================================================================
void FindItemsResultModel::setPackage(PackageInfo* aPackage)
{
    // Check Package
    if (mPackage != aPackage) {
        // Get Was Running
        bool wasRunning = (mWorker && mWorker->isRunning());
        // Stop Worker
        stopWorker();
        // Set Package
        mPackage = aPackage;
        // Clear
        clear();

        // Check Worker
        if (wasRunning) {
            // Start Worker
            startWorker();
        }
    }
}

//==============================================================================
// Get Busy State
//==============================================================================
bool FindItemsResultModel::busy()
{
    return (mWorker && mWorker->mSearchRunning);
}

//==============================================================================
// Get Search Term
//==============================================================================
QString FindItemsResultModel::searchTerm()
{
    return mSearchTerm;
}

//==============================================================================
// Set Search Term
//==============================================================================
void FindItemsResultModel::setSearchTerm(const QString& aSearchTerm)
{
    // Check Search Term
    if (mSearchTerm != aSearchTerm) {
        // Get Was Running
        bool wasRunning = (mWorker && mWorker->isRunning());
        // Stop Worker
        stopWorker();
        // Set Search Term
        mSearchTerm = aSearchTerm;
        // Emit Search Term Changed Signal
        emit searchTermChanged(mSearchTerm);

        // Clear
        clear();

        // Check Worker
        if (wasRunning) {
            // Start Worker
            startWorker();
        }
    }
}

//==============================================================================
// Get Case Sensitive
//==============================================================================
bool FindItemsResultModel::caseSensitive()
{
    return mCaseSensitive;
}

//==============================================================================
// Set Case Sensitive
//==============================================================================
void FindItemsResultModel::setCaseSensitive(const bool& aCaseSensitive)
{
    // Check Case Sensitive
    if (mCaseSensitive != aCaseSensitive) {
        // Get Was Running
        bool wasRunning = (mWorker && mWorker->isRunning());
        // Stop Worker
        stopWorker();
        // Set Case Sensitive
        mCaseSensitive = aCaseSensitive;
        // Emit Case Sensitive Changed Signal
        emit caseSensitiveChanged(mCaseSensitive);

        // Clear
        clear();

        // Check Worker
        if (wasRunning) {
            // Start Worker
            startWorker();
        }
    }
}

//==============================================================================
// Get Whole Words
//==============================================================================
bool FindItemsResultModel::wholeWords()
{
    return mWholeWords;
}

//==============================================================================
// Set Whole Words
//==============================================================================
void FindItemsResultModel::setWholeWords(const bool& aWholeWords)
{
    // Check Whole Words
    if (mWholeWords != aWholeWords) {
        // Get Was Running
        bool wasRunning = (mWorker && mWorker->isRunning());
        // Stop Worker
        stopWorker();
        // Set Whole Words
        mWholeWords = aWholeWords;
        // Emit Whole Words Changed Signal
        emit wholeWordsChanged(mWholeWords);

        // Clear
        clear();

        // Check Worker
        if (wasRunning) {
            // Start Worker
            startWorker();
        }
    }
}

//==============================================================================
// Start Search
//==============================================================================
void FindItemsResultModel::startSearch()
{
    qDebug() << "FindItemsResultModel::startSearch";

    // StartWorker
    startWorker();

    // ...
}

//==============================================================================
// Stop Search
//==============================================================================
void FindItemsResultModel::stopSearch()
{
    qDebug() << "FindItemsResultModel::stopSearch";

    // Stop Worker
    stopWorker();

    // ...

}

//==============================================================================
// Row Count
//==============================================================================
int FindItemsResultModel::rowCount(const QModelIndex& ) const
{
    return mResults.count();
}

//==============================================================================
// Data
//==============================================================================
QVariant FindItemsResultModel::data(const QModelIndex& index, int role) const
{
    // Get Row Index
    int rowIndex = index.row();

    // Check Row Index
    if (rowIndex >= 0 && rowIndex < rowCount()) {
        // Get Result
        ResultItem* item = mResults.at(rowIndex);
        // Check Item
        if (item) {
            // Switch Role
            switch (role) {
                case EFIRSourceIndex:   return item->mIndex;
                case EFIRLabelID:       return item->mLabelID;
                case EFIROriginalText:  return item->mOriginalText;
                case EFIRMatches:       return item->mMatches;

                default:
                break;
            }
        }
    }

    return "";
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> FindItemsResultModel::roleNames() const
{
    // Init Role Names
    QHash<int, QByteArray> rNames;

    rNames[EFIRSourceIndex]     = "sourceIndex";
    rNames[EFIRLabelID]         = "sourceLabelID";
    rNames[EFIROriginalText]    = "sourceOriginalText";
    rNames[EFIRMatches]         = "sourceMatches";

    return rNames;
}

//==============================================================================
// Destructor
//==============================================================================
FindItemsResultModel::~FindItemsResultModel()
{
    // Shut Down
    shutDown();

    // ...
}


























//==============================================================================
// Constructor
//==============================================================================
FindItemsWorker::FindItemsWorker(FindItemsResultModel* aResultModel, QObject* aParent)
    : QThread(aParent)
    , mResultModel(aResultModel)
    , mSearchTerm("")
    , mCaseSensitive(false)
    , mWholeWords(false)
    , mSearchRunning(false)
{
}

//==============================================================================
// Stop
//==============================================================================
void FindItemsWorker::stop()
{
    // Check If Runnind
    if (mSearchRunning) {
        qDebug() << "FindItemsWorker::stop";

        // Terminate
        terminate();

        // Reset Search Running
        mSearchRunning = false;
        // Emit Search Aborted Signal
        emit searchAborted();
    }
}

//==============================================================================
// Run
//==============================================================================
void FindItemsWorker::run()
{
    // Check Result Model
    if (!mResultModel) {
        return;
    }

    // Clear Previous Results
    mResultModel->clear();
    // Sleep a bit
    //msleep(100);

    // Check Package
    if (!mResultModel->mPackage) {
        return;
    }

    // Check Search Term
    if (mResultModel->mSearchTerm.isEmpty()) {
        return;
    }

    // Set Search Running
    mSearchRunning = true;
    // Emit Search Started Signal
    emit searchStarted();

    qDebug() << "FindItemsWorker::run - STARTED.";

    // Set Termination Enabled
    QThread::setTerminationEnabled(true);

//    qDebug() << "###############################";
//    qDebug() << "#### FindItemsWorker::run #####";
//    qDebug() << "###############################";

    // Get Search Term
    mSearchTerm = mResultModel->mSearchTerm;
    // Get Case Sensitive
    mCaseSensitive = mResultModel->mCaseSensitive;
    // Get Whole Words
    mWholeWords = mResultModel->mWholeWords;

    // Get Package Item Count
    int piCount = mResultModel->mPackage->count();

    qDebug() << "FindItemsWorker::run - piCount: " << piCount;

    // Iterate Through Items
    for (int i=0; i<piCount; i++) {
        // Get Translation Item
        TranslationItem* item = mResultModel->mPackage->getItem(i);

        // Check Item
        if (item) {
            //qDebug() << "FindItemsWorker::run - item[" << i << "] ID: " << item->textID() << " - original: " << item->originalText();

            // Check Item For Matches
            QString matches = checkItem(item);

            // Check Item
            if (!matches.isEmpty()) {
                // Emit Add Result Item Signal
                emit addResultItem(i, item->textID(), item->originalText(), matches);
                // Add New Result Item
                //mResultModel->addResultItem(i, item->textID(), item->originalText(), matches);
            }

            // ...
        }
    }

    // ...

    // Reset Search Running
    mSearchRunning = false;
    // Emit SearchFinished Signal
    emit searchFinished();

    qDebug() << "FindItemsWorker::run - FINISHED.";
}

//==============================================================================
// Check Item
//==============================================================================
QString FindItemsWorker::checkItem(TranslationItem* aItem)
{
    // Check Item
    if (aItem && mResultModel && mResultModel->mPackage) {
        // Init Matches Result
        QStringList matches;

        // Get Translations Count
        int tCount = aItem->translations().count();

        // Check Whole Words
        if (mWholeWords) {
            // Check Original
            if (aItem->originalText().compare(mSearchTerm, mCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0) {
                // Append Original Text
                matches << aItem->originalText();
            }

            // Check Translations

            // Iterate Through Translations
            for (int i=0; i<tCount; i++) {
                // Check Translation
                if (aItem->translations()[i].compare(mSearchTerm, mCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0) {
                    // Append Original Text
                    matches << aItem->translations()[i];
                }
            }

        } else {
            // Check Original Text
            if (aItem->originalText().contains(mSearchTerm, mCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive)) {
                // Append Original Text
                matches << aItem->originalText();
            }

            // Check Translations

            // Iterate Through Translations
            for (int j=0; j<tCount; j++) {
                // Check Translation
                if (aItem->translations()[j].contains(mSearchTerm, mCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive)) {
                    // Append Original Text
                    matches << aItem->translations()[j];
                }
            }
        }

        return matches.join(";");
    }

    return "";
}

//==============================================================================
// Destructor
//==============================================================================
FindItemsWorker::~FindItemsWorker()
{
    // Quit
    quit();
}
