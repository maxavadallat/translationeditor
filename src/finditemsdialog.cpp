#include <QQmlEngine>
#include <QQmlContext>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

#include "preferences.h"
#include "packageinfo.h"
#include "src/finditemsdialog.h"
#include "ui_finditemsdialog.h"
#include "finditemsresultmodel.h"
#include "constants.h"

// Max Search Terms
#define FIND_ITEMS_DIALOG_SEARCH_TERM_MAX                   10

// Default Sizes
#define FIND_ITEMS_DIALOG_WIDTH_DEFAULT                     528
#define FIND_ITEMS_DIALOG_HEIGHT_DEFAULT                    116
#define FIND_ITEMS_DIALOG_HEIGHT_RESULTS                    372

//==============================================================================
// Constructor
//==============================================================================
FindItemsDialog::FindItemsDialog(PackageInfo* aPackage, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::FindItemsDialog)
    , mPreferences(Preferences::getInstance())
    , mPackage(aPackage)
    , mPrevState(EFIIdle)
    , mState(EFIIdle)
    , mOKButton(nullptr)
    , mCloseButton(nullptr)
    , mResetButton(nullptr)
    , mResults(nullptr)
    , mAborted(false)
    , mSearchTerm("")
{
    // Setup UI
    ui->setupUi(this);

    // Get OK Button
    mOKButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    // Get Close Button
    mCloseButton = ui->buttonBox->button(QDialogButtonBox::Close);
    // Get Reset Button
    mResetButton = ui->buttonBox->button(QDialogButtonBox::Reset);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void FindItemsDialog::init()
{
    qDebug() << "FindItemsDialog::init";

    // Set Window Flags
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Load History
    loadHistory();

    // Update Buttons
    updateButtons();
    // Update Dialog
    updateDialog();

    // Set Ownership
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Check Search Results
    if (!mResults) {
        // Create Search Results
        mResults = new FindItemsResultModel(mPackage);
        // Connect Signal
        connect(mResults, &FindItemsResultModel::busyChanged, this, &FindItemsDialog::resultsBusyChanged);
    }

    // Get QML Engine
    QQmlEngine* engine = ui->resultsQuickWidget->engine();
    // Get QML Context
    QQmlContext* qmlContext = engine ? engine->rootContext() : nullptr;

    // Check QML Context
    if (qmlContext) {
        // Set Context Property - Preferences
        qmlContext->setContextProperty(CONTEXT_PROPERTY_PREFERENCES, mPreferences);
        // Set Context Property - Search Controller
        qmlContext->setContextProperty(CONTEXT_PROPERTY_SEARCH_CONTROLLER, this);
        // Set Context Property - Search Result Model
        qmlContext->setContextProperty(CONTEXT_PROPERTY_SEARCH_RESULT_MODEL, mResults);

        // ...
    }

    // Set Results Quick Widget Source
    ui->resultsQuickWidget->setSource(QUrl(SEARCH_RESULTS_SOURCE));

    // ...
}

//==============================================================================
// Load History
//==============================================================================
void FindItemsDialog::loadHistory()
{
    qDebug() << "FindItemsDialog::loadHistory";

    // Init Search History File Path
    QString searchHistoryPath = QDir::homePath() + "/" + DEFAULT_SEARCH_HISTORY_NAME;

    // Init Search History File
    QFile searchHistory(searchHistoryPath);

    // Init Search History Items
    QStringList items;

    // Check If File Exists
    if (searchHistory.exists()) {
        // Open Search History
        if (searchHistory.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Init Text Stream
            QTextStream readStream(&searchHistory);
            // Read All
            items = readStream.readAll().split("\n");
            // Close Search History File
            searchHistory.close();
            // Load Items To Search Combo
            ui->searchTermCombo->insertItems(0, items);
        }
    }

    // ...

}

//==============================================================================
// Save History
//==============================================================================
void FindItemsDialog::saveHistory()
{
    qDebug() << "FindItemsDialog::saveHistory";

    // Init Search History File Path
    QString searchHistoryPath = QDir::homePath() + "/" + DEFAULT_SEARCH_HISTORY_NAME;

    // Init Search History File
    QFile searchHistory(searchHistoryPath);

    // Get Count
    int hCount = ui->searchTermCombo->count();

    // Check Search History
    if (hCount > 0) {
        // Open Search History
        if (searchHistory.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Init Write Stream
            QTextStream writeStream(&searchHistory);
            // Init Items Export
            QStringList items;
            // Iterate Through Items
            for (int i=0; i<hCount; i++) {
                // Get Item Data
                items << ui->searchTermCombo->itemText(i);
            }
            // Write to Stream
            writeStream << items.join('\n');

            // Flush
            writeStream.flush();
            // Close File
            searchHistory.close();
        }
    } else {
        // Remove Search History
        searchHistory.remove();
    }

    // ...

}

//==============================================================================
// Insert Current Search Term
//==============================================================================
void FindItemsDialog::insertCurrentTerm()
{
    qDebug() << "FindItemsDialog::insertCurrentTerm - mSearchTerm: " << mSearchTerm;

    // Check Current Search Term
    if (!mSearchTerm.isEmpty()) {
        // Get Count
        int iCount = ui->searchTermCombo->count();
        // Init Drop Down Items
        QStringList items;
        // Iterate Through Items
        for (int i=0; i<iCount; i++) {
            // Append Item Text
            items << ui->searchTermCombo->itemText(i);
        }

        // Get Search Term Index
        int stIndex = items.indexOf(mSearchTerm);

        // Check Index
        if (stIndex >= 0) {
            // Remove Item
            items.removeAt(stIndex);
        }

        // Insert Item
        items.insert(0, mSearchTerm);

        // Check Number Of Items
        if (items.count() > FIND_ITEMS_DIALOG_SEARCH_TERM_MAX) {
            // Remove Last
            items.removeLast();
        }

        // Clear
        ui->searchTermCombo->clear();
        // (Re)Insert Items
        ui->searchTermCombo->insertItems(0, items);
    }
}

//==============================================================================
// Update Buttons
//==============================================================================
void FindItemsDialog::updateButtons()
{
    qDebug() << "FindItemsDialog::updateButtons";

    // Switch State
    switch (mState) {
        case EFIIdle:
            // Set Enabled
            mOKButton->setEnabled(true);
            // Set Button Text
            mOKButton->setText("Search");
            // Set CLose Button Text
            mCloseButton->setText("Close");
            // Set Disabled
            mResetButton->setEnabled(false);
            // Set Enabled State
            ui->searchTermCombo->setEnabled(true);
            // Set Enabled State
            ui->caseSensitiveCheckBox->setEnabled(true);
            // Set Enabled State
            ui->wholeWordsCheckBox->setEnabled(true);
        break;

        case EFISearching:
            // Set Diabled
            mOKButton->setEnabled(false);
            // Set CLose Button Text
            mCloseButton->setText("Stop");
            // Set Disabled
            mResetButton->setEnabled(false);
            // Set Enabled State
            ui->searchTermCombo->setEnabled(false);
            // Set Enabled State
            ui->caseSensitiveCheckBox->setEnabled(false);
            // Set Enabled State
            ui->wholeWordsCheckBox->setEnabled(false);
        break;

        case EFIReady:
            // Set Enabled
            mOKButton->setEnabled(true);
            // Set Button Text
            mOKButton->setText("Search");
            // Set CLose Button Text
            mCloseButton->setText("Close");
            // Set Enabled
            mResetButton->setEnabled(true);
            // Set Enabled State
            ui->searchTermCombo->setEnabled(true);
            // Set Enabled State
            ui->caseSensitiveCheckBox->setEnabled(true);
            // Set Enabled State
            ui->wholeWordsCheckBox->setEnabled(true);
        break;
    }
}

//==============================================================================
// Update Dialog
//==============================================================================
void FindItemsDialog::updateDialog()
{
    qDebug() << "FindItemsDialog::updateDialog";

    // Switch State
    switch (mState) {
        case EFIReady:
            // Show Separator
            ui->horizontalSeparator->setVisible(true);
            // Show Results Widget
            ui->resultsQuickWidget->setVisible(true);
            // Set Spacer Size Policy
            ui->verticalSpacer->changeSize(40, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

            // Resize
            resize(width(), FIND_ITEMS_DIALOG_HEIGHT_RESULTS);
            // Set Min/Max Size
            setMinimumHeight(FIND_ITEMS_DIALOG_HEIGHT_RESULTS);
            setMaximumHeight(FIND_ITEMS_DIALOG_HEIGHT_RESULTS);
        break;

        default:
            // Check Previous State & State
            if (!(mPrevState == EFIReady && mState == EFISearching)) {
                // Hide Separator
                ui->horizontalSeparator->setVisible(false);
                // Hide Results Widget
                ui->resultsQuickWidget->setVisible(false);
                // Set Spacer Size Policy
                ui->verticalSpacer->changeSize(40, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

                // Resize
                resize(width(), FIND_ITEMS_DIALOG_HEIGHT_DEFAULT);
                // Set Min/Max Size
                setMinimumHeight(FIND_ITEMS_DIALOG_HEIGHT_DEFAULT);
                setMaximumHeight(FIND_ITEMS_DIALOG_HEIGHT_DEFAULT);
            }
        break;
    }
}

//==============================================================================
// Reset Dialog
//==============================================================================
void FindItemsDialog::resetDialog()
{
    qDebug() << "FindItemsDialog::resetDialog";

    // Set State
    setState(EFIIdle);

    // Reset Width
    resize(FIND_ITEMS_DIALOG_WIDTH_DEFAULT, height());

    // Reset Case Sensitive Check Box
    ui->caseSensitiveCheckBox->setChecked(false);
    // Reset Whole Words Check Box
    ui->wholeWordsCheckBox->setChecked(false);

    // ...
}

//==============================================================================
// Set State
//==============================================================================
void FindItemsDialog::setState(const EFIStateType& aState)
{
    // Check State
    if (mState != aState) {
        qDebug() << "FindItemsDialog::setState - mState: " << mState << " - aState: " << aState;

        // Set Previous State
        mPrevState = mState;

        // Set State
        mState = aState;

        // Update Buttons
        updateButtons();

        // Update Dialog Size & Result View Visibility
        updateDialog();

        // ...
    }
}

//==============================================================================
// Set Package Info
//==============================================================================
void FindItemsDialog::setPackage(PackageInfo* aPackage)
{
    // Check Package
    if (mPackage != aPackage) {
        // Reset Dialog
        resetDialog();

        // Set Package
        mPackage = aPackage;

        // Check Results Item Model
        if (mResults) {
            // Set Package
            mResults->setPackage(mPackage);
        }

        // ...
    }
}

//==============================================================================
// Results Busy State Changed Slot
//==============================================================================
void FindItemsDialog::resultsBusyChanged(const bool& aBusy)
{
    qDebug() << "FindItemsDialog::resultsBusyChanged - aBusy: " << aBusy;

    // Check Busy State
    if (aBusy) {
        // Set State
        setState(EFISearching);

    } else {
        // Check Aborted State
        if (mAborted) {
            // Set State
            setState(EFIIdle);
        } else {
            // Set State
            setState(EFIReady);
        }
    }
}

//==============================================================================
// Button Box Clicked Slot
//==============================================================================
void FindItemsDialog::on_buttonBox_clicked(QAbstractButton* aButton)
{
    // Check Button
    if (aButton) {
        qDebug() << "FindItemsDialog::on_buttonBox_clicked - mState: " << mState;
        // Check Button
        if (aButton == mOKButton) {
            qDebug() << "FindItemsDialog::on_buttonBox_clicked - OK";

            // Insert Current Term To Combo
            insertCurrentTerm();
            // Save History
            saveHistory();

            // Set Search Term
            mResults->setSearchTerm(mSearchTerm);
            // Start Search
            mResults->startSearch();

            // ...

        } else if (aButton == mCloseButton) {
            qDebug() << "FindItemsDialog::on_buttonBox_clicked - CLOSE";

            // Switch State
            switch (mState) {
                default:
                    // Close Dialog
                    close();
                break;

                case EFISearching:
                    // Set Aborted State
                    mAborted = true;
                    // Stop Search
                    mResults->stopSearch();
                break;
            }
        } else if (aButton == mResetButton) {
            qDebug() << "FindItemsDialog::on_buttonBox_clicked - RESET";

            // Reset Dialog
            resetDialog();

        } else {

            // ...

        }

        // ...

    }
}

//==============================================================================
// On Search Term Combo Activated Slot
//==============================================================================
void FindItemsDialog::on_searchTermCombo_activated(const QString& arg1)
{
    qDebug() << "FindItemsDialog::on_searchTermCombo_activated - arg1: " << arg1;

    // Set Search Term
    mSearchTerm = arg1;

    // ...

}

//==============================================================================
// On Edit Text Changed Slot
//==============================================================================
void FindItemsDialog::on_searchTermCombo_editTextChanged(const QString& arg1)
{
    qDebug() << "FindItemsDialog::on_searchTermCombo_editTextChanged - arg1: " << arg1;

    // Set Search Term
    mSearchTerm = arg1;

    // ...
}

//==============================================================================
// On Case Sensitive Checkbox Toggled Slot
//==============================================================================
void FindItemsDialog::on_caseSensitiveCheckBox_toggled(bool checked)
{
    // Check Results Model
    if (mResults) {
        // Set Case Senstive
        mResults->setCaseSensitive(checked);
    }
}

//==============================================================================
// On Whole Words Checkbox Toggled Slot
//==============================================================================
void FindItemsDialog::on_wholeWordsCheckBox_toggled(bool checked)
{
    // Check Results Model
    if (mResults) {
        // Set Whole Words
        mResults->setWholeWords(checked);
    }
}

//==============================================================================
// Destructor
//==============================================================================
FindItemsDialog::~FindItemsDialog()
{
    // Delete Quick Widgets First
    delete ui->resultsQuickWidget;
    // Delete UI
    delete ui;

    // Check Results
    if (mResults) {
        delete mResults;
    }

    // Check Preferences
    if (mPreferences) {
        // Release Instance
        mPreferences->release();
    }
}

