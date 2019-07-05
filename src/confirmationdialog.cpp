#include "src/confirmationdialog.h"
#include "ui_confirmationdialog.h"

//==============================================================================
// Constructor
//==============================================================================
ConfirmationDialog::ConfirmationDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::ConfirmationDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Set Title
//==============================================================================
void ConfirmationDialog::setTitle(const QString& aTitle)
{
    setWindowTitle(aTitle);
}

//==============================================================================
// Set Confirm Text
//==============================================================================
void ConfirmationDialog::setConfirmText(const QString& aConfirmText)
{
    ui->confirmationTextLabel->setText(aConfirmText);
}

//==============================================================================
// Destructor
//==============================================================================
ConfirmationDialog::~ConfirmationDialog()
{
    delete ui;
}
