#include "tabcorner.h"
#include "ui_tabcorner.h"

TabCorner::TabCorner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabCorner)
{
    ui->setupUi(this);
}

TabCorner::~TabCorner()
{
    delete ui;
}

void TabCorner::on_btnNew_pressed()
{
    emit pressNew();
}


void TabCorner::on_btnMenu_pressed()
{
    emit pressMenu();
}


void TabCorner::on_btnClose_pressed()
{
    emit pressClose();
}

