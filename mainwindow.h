#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socket.h"
#include "fileofstringlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void state(QString);
    void slotTabCloseRequested(int index);
    void tabCreate(QString regExpPattern, QString name);
    void newTab(bool check);

private slots:
    void on_btSend_clicked();

private:
    Ui::MainWindow *ui;
    Socket *rtt_telnet;

    FileOfStringList Tabs = {"tabs.txt"};
    FileOfStringList Patterns = {"patterns.txt"};
    FileOfStringList Commands = {"commands.txt"};
signals:
    void onCommand(QString);
};
#endif // MAINWINDOW_H
