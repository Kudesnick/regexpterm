#include "newtabdialog.h"
#include "ui_newtabdialog.h"

newTabDialog::newTabDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newTabDialog)
{
    ui->setupUi(this);
}

newTabDialog::~newTabDialog()
{
    delete ui;
}

QString newTabDialog::regEx() const
{
    return ui->lineEdit->text();
}
