#include "slot.h"
#include "socket.h"

Slot::Slot(Socket *_slot, QObject *_parent) : QObject(_parent), slot_(static_cast<QObject *>(_slot))
{
    connect(_slot, &Socket::receive , this , &Slot::slotReceive );
    connect(_slot, &Socket::transmit, this , &Slot::slotTransmit);
    connect(_slot, &Socket::stateMsg, this , &Slot::slotStateMsg);
    connect(_slot, &Socket::state   , this , &Slot::slotState   );
    connect(this , &Slot::write     , _slot, &Socket::send      );
}

Slot::Slot(Comport *_slot, QObject *_parent) : QObject(_parent), slot_(static_cast<QObject *>(_slot))
{
    connect(_slot, &Comport::receive , this , &Slot::slotReceive );
    connect(_slot, &Comport::transmit, this , &Slot::slotTransmit);
    connect(_slot, &Comport::stateMsg, this , &Slot::slotStateMsg);
    connect(_slot, &Comport::state   , this , &Slot::slotState   );
    connect(this , &Slot::write      , _slot, &Comport::send     );
}


void Slot::send(QString _str)
{
    emit write(_str);
}

void Slot::slotReceive(QString arg)
{
    emit receive(arg);
}

void Slot::slotTransmit(QString arg)
{
    emit transmit(arg);
}

void Slot::slotStateMsg(QString arg)
{
    emit stateMsg(arg);
}

void Slot::slotState(bool arg)
{
    emit state(arg);
}
