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

    void state(QString);
    void slotTabClosRequested(int index);
    void tabCreate(QString regExpPattern, QString name);
    void newTab(bool check);

private:
    Ui::MainWindow *ui;
    Socket *rtt_telnet;
    bool readStringListFromFile(QString filename, QStringList &strList);
    void writeStringListFromFile(QString filename, QStringList &strList);

    const QString fTabs = "tabs.txt";
    const QString fPatterns = "patterns.txt";
    const QString fCommands = "commands.txt";
};
#endif // MAINWINDOW_H
