#ifndef FRAME_H
#define FRAME_H

#include <QWidget>
#include "socket.h"
#include "settings.h"

namespace Ui { class FrameWindow; }

class Frame : public QWidget
{
    Q_OBJECT

public:
    Frame(QWidget *parent = nullptr);
    ~Frame();
    void closeEvent(QCloseEvent *event);

    void state(QString);
    void slotTabCloseRequested(int index);
    void tabCreate(QString regExpPattern, QString name);
    void newTab(bool check);
    QString printPreamble(bool echo = false);

private slots:
    void on_btSend_clicked();

private:
    Ui::FrameWindow *ui;
    Socket *rtt_telnet;
    Settings *sett;
    bool eventFilter(QObject *target, QEvent *event);
signals:
    void onCommand(QString);
};
#endif // FRAME_H
