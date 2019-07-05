#ifndef FINDITEMSDIALOG_H
#define FINDITEMSDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class FindItemsDialog;
}

class PackageInfo;
class Preferences;
class FindItemsResultModel;

//==============================================================================
// Find Items Dialog Class
//==============================================================================
class FindItemsDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit FindItemsDialog(PackageInfo* aPackage, QWidget* aParent = nullptr);

    // Set Package Info
    void setPackage(PackageInfo* aPackage);

    // Destructor
    ~FindItemsDialog();

signals:
    // Result Item Selected Signal
    void resultItemSelected(const int& aIndex);

protected:
    // State Type
    enum EFIStateType {
        EFIIdle = 0,
        EFISearching,
        EFIReady
    };

    // Init
    void init();

    // Load History
    void loadHistory();
    // Save History
    void saveHistory();

    // Insert Current Search Term
    void insertCurrentTerm();

    // Update Buttons
    void updateButtons();
    // Update Dialog
    void updateDialog();

    // Reset Dialog
    void resetDialog();

    // Set State
    void setState(const EFIStateType& aState);

private slots:
    // Results Busy State Changed Slot
    void resultsBusyChanged(const bool& aBusy);

    // Button Box Clicked Slot
    void on_buttonBox_clicked(QAbstractButton* aButton);

    // On Search Term Combo Activated Slot
    void on_searchTermCombo_activated(const QString& arg1);
    // On Edit Text Changed Slot
    void on_searchTermCombo_editTextChanged(const QString &arg1);

    // On Case Sensitive Checkbox Toggled Slot
    void on_caseSensitiveCheckBox_toggled(bool checked);
    // On Whole Words Checkbox Toggled Slot
    void on_wholeWordsCheckBox_toggled(bool checked);

private:
    // UI
    Ui::FindItemsDialog*    ui;
    // Preferences
    Preferences*            mPreferences;
    // Current Package Info
    PackageInfo*            mPackage;

    // Previous State
    EFIStateType            mPrevState;
    // Dialog State
    EFIStateType            mState;

    // OK Button Reference
    QAbstractButton*        mOKButton;
    // Close Button
    QAbstractButton*        mCloseButton;
    // Reset Button
    QAbstractButton*        mResetButton;

    // Results Model
    FindItemsResultModel*   mResults;

    // Aborted
    bool                    mAborted;

    // Search Term
    QString                 mSearchTerm;
};

#endif // FINDITEMSDIALOG_H
