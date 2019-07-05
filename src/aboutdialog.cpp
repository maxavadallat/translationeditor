#include "aboutdialog.h"
#include "ui_aboutdialog.h"

//==============================================================================
// Constructor
//==============================================================================
AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Destructor
//==============================================================================
AboutDialog::~AboutDialog()
{
    delete ui;
}
