#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "newtabdialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>

void MainWindow::FakeSignal(QString msg)
{
    ui->statusBar->showMessage(msg);
//    ui->txOut->output(msg);
}

void MainWindow::slotTabClosRequested(int index)
{
    if (QMessageBox::question(this, "Delete tab.",
        "Are you shure want to delete tab \"" + ui->tabWidget->tabText(index) + "\"?",
        QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
    {
        ui->tabWidget->removeTab(index);
    }
}

void MainWindow::tabCreate(QString regExpPattern, QString name)
{
    Console *con = new Console(nullptr, regExpPattern);
    ui->tabWidget->addTab(con, name);

    con->output("Hello word!");

    connect(con       , &Console::onCommand, rtt_telnet, &Socket::send          );
    connect(rtt_telnet, &Socket::state     , this      , &MainWindow::FakeSignal);
    connect(rtt_telnet, &Socket::output    , con       , &Console::output       );
}

void MainWindow::newTab(bool check)
{
    (void)check;

    newTabDialog *dlg = new newTabDialog;

    if (dlg->exec() == QDialog::Accepted)
    {
        tabCreate(dlg->regEx(), dlg->regEx());
    }

    delete dlg;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // User code begin

    QPushButton *corner = new QPushButton("new");
    ui->tabWidget->setCornerWidget(corner, Qt::TopRightCorner);
    connect(corner, &QPushButton::clicked, this, &MainWindow::newTab);

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested  , this, &MainWindow::slotTabClosRequested);

    QStringList tabs =
    {
        ".*",
        "^Run.*",
    };

    rtt_telnet = new Socket("localhost", 19021);
    for(auto i : tabs) tabCreate(i, i);

    // User code end
}

MainWindow::~MainWindow()
{
    delete ui;
}

