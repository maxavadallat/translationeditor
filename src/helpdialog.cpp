#include "src/helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
