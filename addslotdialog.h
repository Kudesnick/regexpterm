#ifndef ADDSLOTDIALOG_H
#define ADDSLOTDIALOG_H

#include <QDialog>

namespace Ui {
class AddSlotDialog;
}

class AddSlotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSlotDialog(QWidget *parent = nullptr);
    ~AddSlotDialog();

private:
    Ui::AddSlotDialog *ui;
};

#endif // ADDSLOTDIALOG_H
