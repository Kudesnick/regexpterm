#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include <QMessageBox>

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // User code begin

    QStringList tabs =
    {
        ".*",
        "^Run.*",
    };

    rtt_telnet = new Socket("localhost", 19021);

    for(auto i : tabs)
    {
        Console *con = new Console(nullptr, i);
        ui->tabWidget->addTab(con, i);

        con->output("Hello word!");

        connect(con       , &Console::onCommand, rtt_telnet, &Socket::send          );
        connect(rtt_telnet, &Socket::state     , this      , &MainWindow::FakeSignal);
        connect(rtt_telnet, &Socket::output    , con       , &Console::output       );
    }

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::slotTabClosRequested);

    // User code end
}

MainWindow::~MainWindow()
{
    delete ui;
}

