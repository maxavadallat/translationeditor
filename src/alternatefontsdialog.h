#ifndef ALTERNATEFONTSDIALOG_H
#define ALTERNATEFONTSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class AlternateFontsDialog;
}

class PackageInfo;

//==============================================================================
// Alternate Font Selection Dialog
//==============================================================================
class AlternateFontsDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit AlternateFontsDialog(PackageInfo* aPackage, QWidget* aParent = nullptr);

    // Set Current Package
    void setCurrentPackage(PackageInfo* aPackage);

    // Get Selected Fonts
    QStringList selectedFonts();
    // Set Selected Fonts
    void setSelectedFonts(const QStringList& aSelectedFonts, const QString& aCurrentFont);

    // Destructor
    ~AlternateFontsDialog();

protected:
    // Init
    void init();

    // Build Font List
    void buildFontList();

    // Update Font List
    void updateFontList();

private slots:
    // List Widget Item Clicked Slot
    void on_fontListWidget_itemClicked(QListWidgetItem* item);

private:
    // UI
    Ui::AlternateFontsDialog*   ui;

    // Current Package
    PackageInfo*                mPackage;
    // Current Font
    QString                     mCurrentFont;
    // Selected Fonts
    QStringList                 mSelectedFonts;
};

#endif // ALTERNATEFONTSDIALOG_H
