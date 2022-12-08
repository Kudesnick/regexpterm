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
    readStringListFromFile(fPatterns, patterns);
    newTabDialog *dlg = new newTabDialog(nullptr, &patterns);

    if (dlg->exec() == QDialog::Accepted)
    {
        tabCreate(dlg->regEx(), dlg->regEx());

        patterns.removeAll(dlg->regEx());
        patterns.append(dlg->regEx());

        writeStringListFromFile(fPatterns, patterns);
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

void MainWindow::writeStringListFromFile(QString filename, QStringList &strList)
{
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QFile::Text))
    {
        for (auto i : strList)
        {
            file.write(i.toUtf8() + '\n');
        }
        file.close();
    }
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
    if (!readStringListFromFile(fTabs, tabs))
        tabs.append(".*");
    for(auto i : tabs) tabCreate(i, i);

    readStringListFromFile(fCommands, Console::history);

    // User code end
}

MainWindow::~MainWindow()
{
    QStringList tabs;
    for (auto i = 0; i < ui->tabWidget->count(); i++)
        tabs.append(ui->tabWidget->tabText(i));
    writeStringListFromFile(fTabs, tabs);
    writeStringListFromFile(fCommands, Console::history);

    delete ui;
}
