#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "src/newtranslationdialog.h"
#include "ui_newtranslationdialog.h"

#include "mainwindow.h"

#include "packageinfo.h"
#include "packagegroup.h"

#include "translationitem.h"

#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
NewTranslationDialog::NewTranslationDialog(MainWindow* aMainWindow, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::NewTranslationDialog)
    , mMainWindow(aMainWindow)
    , mCurrentPackage(nullptr)
    , mCurrentGroup(nullptr)
    , mTranslationItem(nullptr)
{
    qDebug() << "NewTranslationDialog created.";

    // Setup UI
    ui->setupUi(this);
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void NewTranslationDialog::init()
{
    // ...
}

//==============================================================================
// Clear
//==============================================================================
void NewTranslationDialog::clear()
{
    qDebug() << "NewTranslationDialog::clear";

    // Clear Group
    ui->groupComboBox->clear();
    // Clear Translation Table Content
    ui->translationTable->clearContents();
}

//==============================================================================
// Cleat Fields
//==============================================================================
void NewTranslationDialog::clearFields()
{
    qDebug() << "NewTranslationDialog::clearFields";

    // Set Text ID
    ui->textIDEdit->setText("");
    // Set Original Text
    ui->originalTextEdit->setText("");
    // Set Screen
    ui->textScreenEdit->setText("");
    // Set State
    ui->statusLabel->setText("");
    // Set Comment
    ui->commentEdit->setText("");

//    // Set Current Index
//    ui->groupComboBox->setCurrentIndex(0);

    // ...
}

//==============================================================================
// Clear Translations
//==============================================================================
void NewTranslationDialog::clearTranslations()
{
    qDebug() << "NewTranslationDialog::clearTranslations";

    // Get Row Count
    int rCount = ui->translationTable->rowCount();
    // Iterate Through Rows
    for (int i=0; i<rCount; i++) {
        // Delete Translation Items
        delete ui->translationTable->takeItem(i, 1);
    }
}

//==============================================================================
// Shut Down
//==============================================================================
void NewTranslationDialog::shutDown()
{
    // Clear
    clear();
}

//==============================================================================
// Override Accept
//==============================================================================
void NewTranslationDialog::accept()
{
    // Check Text ID
    if (ui->textIDEdit->text().isEmpty() || ui->textIDEdit->text().indexOf(" ") != -1) {
        qWarning() << "NewTranslationDialog::accept - Text ID Must be Set!!";
        // Set Style Sheet
        ui->textIDEdit->setStyleSheet("background-color:  \"#44770000\";\n");
        return;
    }

    // Check Current Package
    if (mCurrentPackage) {
        // Find Translation Item
        TranslationItem* item = mCurrentPackage->findItem(ui->textIDEdit->text());

        // Check Text ID
        if (item && item != mTranslationItem) {
            qWarning() << "NewTranslationDialog::accept - Text ID Is in USE!!";
            // Set Style Sheet
            ui->textIDEdit->setStyleSheet("background-color:  \"#44770000\";\n");

            return;
        }
    }

    // Call Base Class Accept
    QDialog::accept();
}

//==============================================================================
// Set Current Package
//==============================================================================
void NewTranslationDialog::setCurrentPackage(PackageInfo* aPackage)
{
    // Check Current Package
    if (mCurrentPackage != aPackage) {
        // Set Current Pacakge
        mCurrentPackage = aPackage;

        // Clear
        clear();

        // Check Current Package
        if (mCurrentPackage) {
            // Get Groups
            QStringList groups = mCurrentPackage->groups();
            // Iterate Through Groups
            for (QString group : groups) {
                // Append groups
                ui->groupComboBox->addItem(group);
            }

            // Set Current Index
            ui->groupComboBox->setCurrentIndex(ui->groupComboBox->count() ? 0 : -1);

            // Get Available Languages
            mLanguages = mCurrentPackage->languages();
            // Get Languages Count
            int lCount = mLanguages.count();

            // Set Row Count
            ui->translationTable->setRowCount(lCount);

            // Iterate Through Languages
            for (int i=0; i<lCount; i++) {
                // Get Language
                QString language = mMainWindow ? mMainWindow->getLanguageName(mLanguages[i]) : mLanguages[i];
                // Get Flag Icon
                QIcon flagIcon(mMainWindow ? mMainWindow->getLanguageFlag(mLanguages[i]) : "");
                // Create New Table Widget Item
                QTableWidgetItem* newLanguageItem = new QTableWidgetItem(flagIcon, language);
                // Set Read Only
                newLanguageItem->setFlags(newLanguageItem->flags() ^ Qt::ItemIsEditable);
                // Set Item
                ui->translationTable->setItem(i, 0, newLanguageItem);
            }

            // Resize Rows To Content
            ui->translationTable->resizeRowsToContents();
        }
    }
}

//==============================================================================
// Set Current Group
//==============================================================================
void NewTranslationDialog::setCurrentGroup(PackageGroup* aGroup)
{
    // Check Current Group
    if (mCurrentGroup != aGroup) {
        // Set Current Group
        mCurrentGroup = aGroup;

        // ...
    }
}

//==============================================================================
// Load Translation Item
//==============================================================================
void NewTranslationDialog::loadTranslationItem(TranslationItem* aItem)
{
    // Check Current Translation Item
    if (mTranslationItem != aItem) {
        // Set Current Translation Item
        mTranslationItem = aItem;

        // Clear Fields
        clearFields();
        // Reset/Clear Translations
        clearTranslations();

        // Check Current Translation Item
        if (mTranslationItem) {
            // Set Text ID
            ui->textIDEdit->setText(mTranslationItem->textID());
            // Set Original Text
            ui->originalTextEdit->setText(mTranslationItem->originalText());
            // Set Screen
            ui->textScreenEdit->setText(mTranslationItem->screen());
            // Set State
            ui->statusLabel->setText(mTranslationItem->textState());
            // Set Comment
            ui->commentEdit->setText(mTranslationItem->comment());

            // Get Item Group
            QString itemGroup = mTranslationItem->translationGroup();

            // Check Current Package
            if (mCurrentPackage) {
                // Get Package Group
                setCurrentGroup(mCurrentPackage->packageGroup(itemGroup));
            }

            // Check Group
            if (!itemGroup.isEmpty()) {
                // Get Groups Count
                int gCount = ui->groupComboBox->count();
                // Init Items
                QStringList gItems;
                // Iterate Through Items
                for (int i=0; i<gCount; i++) {
                    // Append Item
                    gItems << ui->groupComboBox->itemText(i);
                }

                // Get Group Name Index
                int gnIndex = gItems.indexOf(itemGroup);
                // Check Group Name Index
                if (gnIndex != -1) {
                    // Set Group Combo Box Current Index
                    ui->groupComboBox->setCurrentIndex(gnIndex);
                }
            }

            // Get Translation Item Languages
            QStringList tLanguages = mTranslationItem->languages();

            // Iterate Through Translation Item Languages
            for (QString tLanguage : tLanguages) {
                // Get Row Index
                int trIndex = mLanguages.indexOf(tLanguage);

                // Create New Table Widget Item
                QTableWidgetItem* newItem = new QTableWidgetItem(mTranslationItem->getTranslation(tLanguage).replace("\n", "\\n"));
                // Set Item
                ui->translationTable->setItem(trIndex, 1, newItem);
            }

            // Resize Rows To Content
            ui->translationTable->resizeRowsToContents();

            // ...

        } else {

            // ...

        }
    }
}

//==============================================================================
// Save Translation Item
//==============================================================================
void NewTranslationDialog::saveTranslationItem()
{
    // Check Current Translation Item
    if (mTranslationItem) {
        // Set Text ID
        mTranslationItem->setTextID(ui->textIDEdit->text());
        // Set Original Text
        mTranslationItem->setOriginalText(ui->originalTextEdit->text());
        // Set Screen
        mTranslationItem->setScreen(ui->textScreenEdit->text());
        // Set Comment
        mTranslationItem->setComment(ui->commentEdit->text());
        // Set Group
        mTranslationItem->setTranslationGroup(ui->groupComboBox->currentText());

        // Get Translation Table Row Count
        int ttRowCount = ui->translationTable->rowCount();
        // Iterate Through Items
        for (int i=0; i<ttRowCount; i++) {
            // Get Table Widget Item
            QTableWidgetItem* item = ui->translationTable->item(i, 1);
            // Check Table Widget Item
            if (item) {
                // Get Translation
                QString translation = item->text();
                // Set Translation
                mTranslationItem->setTranslation(mLanguages[i], translation.replace("\\n", "\n"));
            }
        }
    }
}

//==============================================================================
// Get Selected Group
//==============================================================================
QString NewTranslationDialog::getSelectedGroup()
{
    return ui->groupComboBox->currentText();
}

//==============================================================================
// Set Group Selector Enabled
//==============================================================================
void NewTranslationDialog::setGroupSelectorEnabled(const bool& aEnabled)
{
    // Set Enabled
    ui->groupComboBox->setEnabled(aEnabled);
}

//==============================================================================
// Set Text ID Editor Focused
//==============================================================================
void NewTranslationDialog::setTextIDEditorForcused()
{
    // Set Focus
    ui->textIDEdit->setFocus();
}

//==============================================================================
// Text ID Edit Text Changed Slot
//==============================================================================
void NewTranslationDialog::on_textIDEdit_textChanged(const QString& arg1)
{
    qDebug() << "NewTranslationDialog::on_textIDEdit_textChanged - arg1: " << arg1;

    // Reset Style Sheet
    ui->textIDEdit->setStyleSheet("");

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
NewTranslationDialog::~NewTranslationDialog()
{
    // Shut Down
    shutDown();

    delete ui;

    qDebug() << "NewTranslationDialog deleted.";
}

