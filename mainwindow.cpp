#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "newtabdialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>

void MainWindow::state(QString msg)
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

    connect(con       , &Console::onCommand, rtt_telnet, &Socket::send     );
    connect(rtt_telnet, &Socket::output    , con       , &Console::output  );
    connect(rtt_telnet, &Socket::state, con, &Console::setConnectState);
}

void MainWindow::newTab(bool check)
{
    (void)check;

    QStringList patterns;
    readStringListFromFile("patterns.txt", patterns);
    newTabDialog *dlg = new newTabDialog(nullptr, &patterns);

    if (dlg->exec() == QDialog::Accepted)
    {
        tabCreate(dlg->regEx(), dlg->regEx());
    }

    delete dlg;
}

bool MainWindow::readStringListFromFile(QString filename, QStringList &strList)
{
    QFile file(filename);

    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        while(!file.atEnd())
        {
            strList.append(QString(file.readLine()).trimmed());
        }
        file.close();
    }

    return (strList.count() != 0);
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

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::slotTabClosRequested);

    rtt_telnet = new Socket("localhost", 19021);
    connect(rtt_telnet, &Socket::stateMsg, this, &MainWindow::state);

    QStringList tabs;
    if (!readStringListFromFile("tabs.txt", tabs))
        tabs.append(".*");
    for(auto i : tabs) tabCreate(i, i);

    readStringListFromFile("commands.txt", Console::history);

    // User code end
}

MainWindow::~MainWindow()
{
    delete ui;
}
