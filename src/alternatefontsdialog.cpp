#include <QDebug>
#include <QListWidgetItem>
#include <QDir>
#include <QFileInfoList>

#include "src/alternatefontsdialog.h"
#include "ui_alternatefontsdialog.h"

#include "packageinfo.h"

//==============================================================================
// Constructor
//==============================================================================
AlternateFontsDialog::AlternateFontsDialog(PackageInfo* aPackage, QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::AlternateFontsDialog)
    , mPackage(aPackage)
    , mCurrentFont("")
{
    qDebug() << "AlternateFontsDialog created.";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void AlternateFontsDialog::init()
{
    // Set Window Flags
    setWindowFlags(Qt::Tool);

    // Add Font Items, Build Font List
    buildFontList();
}

//==============================================================================
// Build Font List
//==============================================================================
void AlternateFontsDialog::buildFontList()
{
    // Check Current Package
    if (mPackage) {
        // Clear Font List Widget
        ui->fontListWidget->clear();

        // Init Font Dir Info
        QFileInfo fontDirInfo(mPackage->fontsDir());

        // Check If Dir Exists
        if (fontDirInfo.isDir()) {
            qDebug() << "AlternateFontsDialog::buildFontList - fontsDir: " << mPackage->fontsDir();

            // Init Font Dir
            QDir fontDir(fontDirInfo.absoluteFilePath());

            // Init Name Filters
            QStringList nameFilters = QString("*.otf, *.ttf").split(",");

            // Get font Info List
            QFileInfoList fontInfoList = fontDir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

            // Iterate Through Font Info List
            for (QFileInfo fileInfo : fontInfoList) {
                // Create New List Widget Item
                QListWidgetItem* newItem = new QListWidgetItem();
                // Set Text
                newItem->setText(fileInfo.fileName());
                // Reset Check State
                newItem->setCheckState(Qt::Unchecked);

                //connect(newItem, &QListWidgetItem::)

                // Append Item
                ui->fontListWidget->addItem(newItem);
            }

            // ...
        }
    }
}

//==============================================================================
// Update Font List
//==============================================================================
void AlternateFontsDialog::updateFontList()
{
    qDebug() << "AlternateFontsDialog::updateFontList";

    // Get Fonts Count
    int fCount = ui->fontListWidget->count();

    // Iterate Through Fonts
    for (int i=0; i<fCount; i++) {
        // Get Item
        QListWidgetItem* fontItem = ui->fontListWidget->item(i);
        // Check Item
        if (fontItem) {
            // Check Item
            if (fontItem->text() == mCurrentFont) {
                // Set Enabled
                fontItem->setFlags(fontItem->flags() ^ Qt::ItemIsEnabled);
                // Set Checked
                fontItem->setCheckState(Qt::PartiallyChecked);
            } else {
                // Set Enabled
                fontItem->setFlags(fontItem->flags() | Qt::ItemIsEnabled);
                // Set Checked
                fontItem->setCheckState(mSelectedFonts.indexOf(fontItem->text()) >= 0 ? Qt::Checked : Qt::Unchecked);
            }
        }
    }
}

//==============================================================================
// Set Current Package
//==============================================================================
void AlternateFontsDialog::setCurrentPackage(PackageInfo* aPackage)
{
    // Check Package
    if (mPackage != aPackage) {
        // Set Package
        mPackage = aPackage;

        // Build Font List
        buildFontList();
    }
}

//==============================================================================
// Get Selected Fonts
//==============================================================================
QStringList AlternateFontsDialog::selectedFonts()
{
    return mSelectedFonts;
}

//==============================================================================
// Set Selected Fonts
//==============================================================================
void AlternateFontsDialog::setSelectedFonts(const QStringList& aSelectedFonts, const QString& aCurrentFont)
{
    // Check Selected Fonts
    if (mSelectedFonts != aSelectedFonts || mCurrentFont != aCurrentFont) {
        qDebug() << "AlternateFontsDialog::setSelectedFonts - aSelectedFonts: " << aSelectedFonts;
        // Set Selected Fonts
        mSelectedFonts = aSelectedFonts;
        // Set Current Font
        mCurrentFont = aCurrentFont;

        // Update Font List
        updateFontList();
    }

}

//==============================================================================
// List Widget Item Clicked Slot
//==============================================================================
void AlternateFontsDialog::on_fontListWidget_itemClicked(QListWidgetItem* item)
{
    // Get Item Text/Font Index
    int fontIndex = mSelectedFonts.indexOf(item->text());

    // Check Item Checked State
    if (item->checkState() == Qt::Checked) {
        // Check Font Index
        if (fontIndex == -1) {
            // Append Font To Selected Fonts
            mSelectedFonts.append(item->text());
            // Sort
            mSelectedFonts.sort();
        }
    } else {
        // Check Font Index
        if (fontIndex != -1) {
            // Remove Selected Font
            mSelectedFonts.removeAt(fontIndex);
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
AlternateFontsDialog::~AlternateFontsDialog()
{
    // Delete UI
    delete ui;

    qDebug() << "AlternateFontsDialog deleted.";
}

