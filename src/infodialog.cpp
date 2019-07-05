#include "src/infodialog.h"
#include "ui_infodialog.h"

//==============================================================================
// Constructor
//==============================================================================
InfoDialog::InfoDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Set Title
//==============================================================================
void InfoDialog::setTitle(const QString& aTitle)
{
    setWindowTitle(aTitle);
}

//==============================================================================
// Set Info Text
//==============================================================================
void InfoDialog::setInfoText(const QString& aInfoText)
{
    ui->infoTextLabel->setText(aInfoText);
}

//==============================================================================
// Destructor
//==============================================================================
InfoDialog::~InfoDialog()
{
    delete ui;
}
