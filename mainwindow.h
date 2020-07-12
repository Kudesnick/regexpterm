#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void FakeSignal(QString);
    void slotTabClosRequested(int index);
    void tabCreate(QString regExpPattern, QString name);
    void newTab(bool check);

private:
    Ui::MainWindow *ui;
    Socket *rtt_telnet;
};
#endif // MAINWINDOW_H
