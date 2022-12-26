#ifndef SLOT_H
#define SLOT_H

#include <QObject>
#include <QString>
#include "socket.h"
#include "comport.h"

class Slot : public QObject
{
    Q_OBJECT
public:
    Slot(Socket *_slot, QObject *_parent = nullptr);
    Slot(Comport *_slot, QObject *_parent = nullptr);
    void send(QString _str);

private:
    QObject *slot_;

    void slotReceive(QString);
    void slotTransmit(QString);
    void slotStateMsg(QString);
    void slotState(bool);

signals:
    void receive(QString);
    void transmit(QString);
    void stateMsg(QString);
    void state(bool);
    void write(QString);
};

#endif // SLOT_H
