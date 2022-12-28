#include "addslotdialog.h"
#include "ui_addslotdialog.h"

AddSlotDialog::AddSlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSlotDialog)
{
    ui->setupUi(this);
}

AddSlotDialog::~AddSlotDialog()
{
    delete ui;
}
