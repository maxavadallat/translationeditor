#ifndef NEWTRANSLATIONDIALOG_H
#define NEWTRANSLATIONDIALOG_H

#include <QDialog>

namespace Ui {
class NewTranslationDialog;
}

class MainWindow;

class PackageInfo;
class PackageGroup;
class TranslationItem;
class TranslationItemLayout;

//==============================================================================
// New/Edit Translation Item Dialog
//==============================================================================
class NewTranslationDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit NewTranslationDialog(MainWindow* aMainWindow, QWidget* aParent = nullptr);

    // Set Current Package
    void setCurrentPackage(PackageInfo* aPackage);
    // Set Current Group
    void setCurrentGroup(PackageGroup* aGroup);
    // Load Translation Item
    void loadTranslationItem(TranslationItem* aItem);
    // Save Translation Item
    void saveTranslationItem();

    // Get Selected Group
    QString getSelectedGroup();

    // Set Group Selector Enabled
    void setGroupSelectorEnabled(const bool& aEnabled);
    // Set Text ID Editor Focused
    void setTextIDEditorForcused();

    // Destructor
    ~NewTranslationDialog();

public slots:
    // Override Accept
    virtual void accept();

protected:
    // Init
    void init();
    // Clear
    void clear();
    // Cleat Fields
    void clearFields();
    // Clear Translations
    void clearTranslations();
    // Shut Down
    void shutDown();

private slots:
    // Text ID Edit Text Changed Slot
    void on_textIDEdit_textChanged(const QString& arg1);

private:
    // UI
    Ui::NewTranslationDialog*   ui;

    // Main Window
    MainWindow*                 mMainWindow;

    // Current Package
    PackageInfo*                mCurrentPackage;
    // Current Group
    PackageGroup*               mCurrentGroup;
    // Translation Item
    TranslationItem*            mTranslationItem;

    // Current Package Languages
    QStringList                 mLanguages;
};

#endif // NEWTRANSLATIONDIALOG_H
