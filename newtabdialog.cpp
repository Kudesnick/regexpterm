#include "newtabdialog.h"
#include "ui_newtabdialog.h"

newTabDialog::newTabDialog(QWidget *parent, QStringList *itemList) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::newTabDialog)
{
    ui->setupUi(this);

    if (itemList != nullptr)
        ui->comboBox->addItems(*itemList);
}

newTabDialog::~newTabDialog()
{
    delete ui;
}

QString newTabDialog::regEx() const
{
    return ui->comboBox->currentText();
}
