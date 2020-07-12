#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::FakeSignal(QString msg)
{
    ui->statusBar->showMessage(msg);
    ui->txOut->output(msg);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // User code begin

    ui->txOut->output("Hello word!");
    rtt_telnet = new Socket("localhost", 19021);

    connect(ui->txOut , &Console::onCommand, rtt_telnet, &Socket::send          );
    connect(rtt_telnet, &Socket::state     , this      , &MainWindow::FakeSignal);
    connect(rtt_telnet, &Socket::output    , ui->txOut , &Console::output       );

    // User code end
}

MainWindow::~MainWindow()
{
    delete ui;
}

