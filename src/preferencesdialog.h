#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QQmlContext>

namespace Ui {
class PreferencesDialog;
}

class Preferences;
class PluginInfo;

//==============================================================================
// Preferences Dialog Class
//==============================================================================
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit PreferencesDialog(QWidget* aParent = nullptr);

    // Load Preferences
    void loadPreferences();

    // Load Defults
    void restoreDefaults();

    // Apply Settings
    void applyPreferences();

    // Save Preferences
    void savePreferences();

    // Destructor
    ~PreferencesDialog();

protected:
    // Init
    void init();
    // Clear
    void clear();
    // Shut Down
    void shutDown();

    // Set Dirty State
    void setDirty(const bool& aDirty = true);

    // Load Plugins List
    void loadPluginsList();

    // Update Buttons
    void updateButtons();
    // Update PLugin List
    void updatePluginList();

    // Add Plugin Info
    void addPluginInfo(const PluginInfo& aPluginInfo, const bool& aUpdateList = true);
    // Remove Plugin Info
    void removePluginInfo(const int& aIndex, const bool& aUpdateList = true);

    // Load Plugin
    PluginInfo loadPlugin(const QString& aPath);

protected slots:
    // Button Clicked Slot
    void buttonClicked(QAbstractButton* aButton);
    // Dirty State Changed Slot
    void dirtyStateChanged(const bool& aDirty);

private slots:
    // On Font Select Button Clicked Slot
    void on_fontSelectButton_clicked();
    // GUI Theme Combo Box Current Index Changed Slot
    void on_themeComboBox_currentIndexChanged(int index);
    // On Custom Colors Button Clicked Slot
    void on_customColorsButton_clicked();
    // Font Dir Select Button Clicked Slot
    void on_fontDirSelectButton_clicked();
    // Font Dir Edit Text Changed Slot
    void on_fontDirEditTextBox_textChanged(const QString &arg1);
    // Qt Dir Select Button Clicked Slot
    void on_qtDirSelectButton_clicked();
    // Qt Dir Edit Text Box Text Changed Slot
    void on_qtDirEditTextBox_textChanged(const QString &arg1);
    // Add Plugin Button Clicked Slot
    void on_addPluginButton_clicked();
    // Remove Plugin Button Clicked Slot
    void on_removePluginButton_clicked();

    // On Color Picker Button Text Clicked Slot
    void on_colorPickerButtonText_clicked();
    // On Color Picker Button Background Clicked Slot
    void on_colorPickerButtonBackground_clicked();
    // On Color Picker Button Border Clicked Slot
    void on_colorPickerButtonBorder_clicked();
    // On Color Picker Button Alternative Row Clicked Slot
    void on_colorPickerButtonAltRow_clicked();
    // On Color Picker Button Header Clicked Slot
    void on_colorPickerButtonHeader_clicked();
    // On Color Picker Button Highlight Clicked Slot
    void on_colorPickerButtonHighlight_clicked();

    // On Font Size Spinner Value Changed Slot
    void on_fontSizeSpinner_valueChanged(int arg1);
    // On Row Height Spinner Value Changed Slot
    void on_rowHeightSpinner_valueChanged(int arg1);
    // On Alternate Row Colors Toggled Slot
    void on_alternateRowColorsCheckBox_toggled(bool checked);

private:
    // UI
    Ui::PreferencesDialog*  ui;
    // Preferences
    Preferences*            mPreferences;
    // Dialog Settings Dirty State
    bool                    mDirty;

    // Last Open Plugin Dir
    QString                 mLastPluginDir;
    // Plugins List
    QList<PluginInfo>       mPluginsList;

    // Custom Colors View
    bool                    mCustomColorsView;
    // Colors Preview Quick Widget Context
    QQmlContext*            mColorsQMLContext;
};

#endif // PREFERENCESDIALOG_H
