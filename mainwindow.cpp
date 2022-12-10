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
}

void MainWindow::slotTabCloseRequested(int index)
{
    if (QMessageBox::question(this, "Delete tab",
        "To delete tab \"" + ui->tabWidget->tabText(index) + "\"?",
        QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
    {
        ui->tabWidget->removeTab(index);
    }
}

void MainWindow::tabCreate(QString regExpPattern, QString name)
{
    Console *con = new Console(nullptr, regExpPattern);
    ui->tabWidget->addTab(con, name);

    connect(rtt_telnet, &Socket::receive , con, &Console::print          );
    /// @todo create  setting echo on/off
    connect(rtt_telnet, &Socket::transmit, con, &Console::print          );
    connect(rtt_telnet, &Socket::state   , con, &Console::setConnectState);
}

void MainWindow::newTab(bool check)
{
    (void)check;

    QStringList patternsList;
    Patterns.read(patternsList);
    newTabDialog *dlg = new newTabDialog(nullptr, &patternsList);

    if (dlg->exec() == QDialog::Accepted)
    {
        tabCreate(dlg->regEx(), dlg->regEx());

        patternsList.removeAll(dlg->regEx());
        patternsList.append(dlg->regEx());

        Patterns.write(patternsList);
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
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::slotTabCloseRequested);

    rtt_telnet = new Socket("localhost", 19021);
    connect(rtt_telnet, &Socket::stateMsg     , this      , &MainWindow::state);
    connect(this      , &MainWindow::onCommand, rtt_telnet, &Socket::send     );

    QStringList tabsList;
    if (!Tabs.read(tabsList))
        tabsList.append(".*");
    for(auto &i : tabsList) tabCreate(i, i);


    QStringList commandList;
    Commands.read(commandList);
    ui->cbCmdline->addItems(commandList);

    // User code end
}

MainWindow::~MainWindow()
{
    QStringList tabsList;
    for (auto i = 0; i < ui->tabWidget->count(); i++)
        tabsList.append(ui->tabWidget->tabText(i));
    Tabs.write(tabsList);

    QStringList commandList;
    for (auto i = 0; i < ui->cbCmdline->count(); i++)
        commandList.append(ui->cbCmdline->itemText(i));
    Commands.write(commandList);

    delete ui;
}

void MainWindow::on_btSend_clicked()
{
    QString eol = "\r"; /// @todo create setting '\r', '\n', '\r\n' or nothing

    emit onCommand(ui->cbCmdline->currentText() + eol);
}
