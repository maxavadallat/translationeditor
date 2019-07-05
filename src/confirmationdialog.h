#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmationDialog;
}

//==============================================================================
// Confirmation Dialog Class
//==============================================================================
class ConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit ConfirmationDialog(QWidget* aParent = nullptr);

    // Set Title
    void setTitle(const QString& aTitle);
    // Set Confirm Text
    void setConfirmText(const QString& aConfirmText);

    // Destructor
    ~ConfirmationDialog();

private:
    // UI
    Ui::ConfirmationDialog *ui;
};

#endif // CONFIRMATIONDIALOG_H
