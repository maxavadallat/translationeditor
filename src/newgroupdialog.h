#ifndef NEWGROUPDIALOG_H
#define NEWGROUPDIALOG_H

#include <QDialog>

namespace Ui {
class NewGroupDialog;
}

//==============================================================================
// New Group Dialog Class
//==============================================================================
class NewGroupDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit NewGroupDialog(QWidget* aParent = nullptr);

    // Get Group Name
    QString groupName();
    // Set Group Name
    void setGroupName(const QString& aGroupName);

    // Destructor
    ~NewGroupDialog();

private:
    // UI
    Ui::NewGroupDialog*     ui;
};

#endif // NEWGROUPDIALOG_H
