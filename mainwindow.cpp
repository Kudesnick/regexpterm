#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frame.h"
#include "socket.h"
#include "comport.h"
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Slot *rtt_telnet = new Slot(new Socket("localhost", 19021), nullptr);
    Slot *serial_port = new Slot(new Comport("COM1", QSerialPort::Baud115200, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop), nullptr);

    QSplitter *split = new QSplitter(Qt::Horizontal, this);
    ui->gridLayout->addWidget(split);
    split->addWidget(new Frame(rtt_telnet, split));
    split->addWidget(new Frame(serial_port, split));
}

MainWindow::~MainWindow()
{
    delete ui;
}
