#ifndef PACKAGEPREFERENCESDIALOG_H
#define PACKAGEPREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PackagePreferencesDialog;
}

class MainWindow;
class Preferences;
class PackageInfo;

//==============================================================================
// Package Preferences Dialog
//==============================================================================
class PackagePreferencesDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(PackageInfo* currentPackage READ currentPackage NOTIFY currentPackageChanged)

public:
    // Constructor
    explicit PackagePreferencesDialog(MainWindow* aMainWidnow, QWidget* aParent = nullptr);

    // Get Current Package
    PackageInfo* currentPackage();

    // Load Package Preferences
    void loadPreferences(PackageInfo* aPackageInfo);
    // Save Preferences
    void savePreferences();
    // Load Defaults
    void loadDefaults();

    // Destructor
    ~PackagePreferencesDialog();

signals:
    // Current Package Changed
    void currentPackageChanged(PackageInfo* aPackageInfo);

private:
    // Init
    void init();
    // ShutDown
    void shutDown();

    // Build Font List
    void buildFontList(const QString& aPath);

protected:
    friend class MainWindow;
    // Set Current Package
    void setCurrentPackage(PackageInfo* aPackageInfo);
    // Discard New Package
    void discardNewPackage();

private slots:
    // On Font Dir Edit Text Changed Slot
    void on_fontDirEdit_textChanged(const QString &arg1);
    // Browse Font Dir Button Clicked Slot
    void on_browseFontDirButton_clicked();

protected:
    // UI
    Ui::PackagePreferencesDialog*   ui;

    // Main Window
    MainWindow*                     mMainWindow;

    // Preferences
    Preferences*                    mPreferences;

    // Current Package Info
    PackageInfo*                    mCurrentPackage;
};

#endif // PACKAGEPREFERENCESDIALOG_H
