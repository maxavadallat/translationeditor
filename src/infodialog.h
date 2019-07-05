#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

//==============================================================================
// Info Dialog Class
//==============================================================================
class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit InfoDialog(QWidget* aParent = nullptr);

    // Set Title
    void setTitle(const QString& aTitle);
    // Set Info Text
    void setInfoText(const QString& aInfoText);

    // Destructor
    ~InfoDialog();

private:
    // UI
    Ui::InfoDialog *ui;
};

#endif // INFODIALOG_H
