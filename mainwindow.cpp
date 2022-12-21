#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frame.h"
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSplitter *split = new QSplitter(Qt::Horizontal, this);
    ui->gridLayout->addWidget(split);
    split->addWidget(new Frame(split));
    split->addWidget(new Frame(split));
}

MainWindow::~MainWindow()
{
    delete ui;
}
