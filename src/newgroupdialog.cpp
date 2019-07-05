#include "src/newgroupdialog.h"
#include "ui_newgroupdialog.h"

//==============================================================================
// Constructor
//==============================================================================
NewGroupDialog::NewGroupDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::NewGroupDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Get Group Name
//==============================================================================
QString NewGroupDialog::groupName()
{
    return ui->groupNameEdit->text();
}

//==============================================================================
// Set Group Name
//==============================================================================
void NewGroupDialog::setGroupName(const QString& aGroupName)
{
    ui->groupNameEdit->setText(aGroupName);
}

//==============================================================================
// Destructor
//==============================================================================
NewGroupDialog::~NewGroupDialog()
{
    delete ui;
}
