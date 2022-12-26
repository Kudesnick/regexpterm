#include "frame.h"
#include "ui_framewindow.h"
#include "console.h"
#include "newtabdialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QTime>

void Frame::state(QString msg)
{
    ui->statusBar->setText(msg);
}

void Frame::slotTabCloseRequested(int index)
{
    if (QMessageBox::question(this, "Delete tab",
        "To delete tab \"" + ui->tabWidget->tabText(index) + "\"?",
        QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
    {
        ui->tabWidget->removeTab(index);
    }
}

void Frame::tabCreate(QString regExpPattern, QString name)
{
    Console *con = new Console(nullptr, regExpPattern);
    ui->tabWidget->addTab(con, name);

    connect(slot, &Slot::receive         , con , &Console::print);
    /// @todo create  setting echo on/off
    connect(slot, &Slot::transmit        , con , &Console::printEcho);
    connect(con , &Console::printPreamble, this, &Frame::printPreamble);
}

void Frame::newTab(bool check)
{
    (void)check;

    QStringList patternsList;
    sett->patterns.read(patternsList);
    newTabDialog *dlg = new newTabDialog(nullptr, &patternsList);

    if (dlg->exec() == QDialog::Accepted)
    {
        tabCreate(dlg->regEx(), dlg->regEx());

        patternsList.removeAll(dlg->regEx());
        patternsList.append(dlg->regEx());

        sett->patterns.write(patternsList);
    }

    delete dlg;
}

QString Frame::printPreamble(bool echo)
{
    QString result = QTime::currentTime().toString("hh:mm:ss.zzz");
    return result + (echo ? "< " : "> ");
}

Frame::Frame(Slot *_slot, QWidget *_parent)
    : QWidget(_parent)
    , ui(new Ui::FrameWindow())
    , slot(_slot)
{
    ui->setupUi(this);

    ui->cbCmdLine->installEventFilter(this);

    // User code begin
    /// @todo добавить возможность выбора имени файла в командной строке при запуске
    sett = new Settings("regexpterm.ini");

    QPushButton *corner = new QPushButton("new");
    ui->tabWidget->setCornerWidget(corner, Qt::TopRightCorner);

    connect(corner, &QPushButton::clicked, this, &Frame::newTab);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &Frame::slotTabCloseRequested);

    /// @todo add to setting
    connect(slot, &Slot::stateMsg  , this, &Frame::state);
    connect(this, &Frame::onCommand, slot, &Slot::send);

    QStringList tabsList;
    if (!sett->tabs.read(tabsList))
        tabsList.append(".*");
    for(auto &i : tabsList) tabCreate(i, i);

    QStringList commandList;
    sett->cmds.read(commandList);
    ui->cbCmdLine->addItems(commandList);

    // User code end
}

Frame::~Frame()
{
    delete ui;
}

void Frame::closeEvent(QCloseEvent *event)
{
    (void)event;

    QStringList tabsList;
    for (auto i = 0; i < ui->tabWidget->count(); i++)
        tabsList.append(ui->tabWidget->tabText(i));
    sett->tabs.write(tabsList);

    QStringList commandList;
    for (auto i = 0; i < ui->cbCmdLine->count(); i++)
        commandList.append(ui->cbCmdLine->itemText(i));
    sett->cmds.write(commandList);
}

void Frame::on_btSend_clicked()
{
    QString eol = "\r"; /// @todo create setting '\r', '\n', '\r\n' or nothing

    emit onCommand(ui->cbCmdLine->currentText() + eol);
}

bool Frame::eventFilter(QObject *target, QEvent *event)
{
    if (true
        && target == ui->cbCmdLine
        && event->type() == QEvent::KeyPress
        )
    {
        int key = static_cast<QKeyEvent *>(event)->key();
        if (key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            on_btSend_clicked();
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}
