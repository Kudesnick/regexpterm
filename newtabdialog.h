#ifndef NEWTABDIALOG_H
#define NEWTABDIALOG_H

#include <QDialog>

namespace Ui {
class newTabDialog;
}

class newTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newTabDialog(QWidget *parent = nullptr, QStringList *itemList = nullptr);
    ~newTabDialog();

    QString regEx() const;

private:
    Ui::newTabDialog *ui;
};

#endif // NEWTABDIALOG_H
