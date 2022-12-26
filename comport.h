#ifndef COMPORT_H
#define COMPORT_H

#include <QSerialPort>

class Comport : public QSerialPort
{
    Q_OBJECT
public:
    explicit Comport(QString _name, enum BaudRate _baud, enum DataBits _dataBits, enum Parity _parity, enum StopBits _stopBits, QObject *_prnt = nullptr);
    void send(QString _Str);

private:
    void slotConnected();
    void slotDisconnected();
    void slotError(QSerialPort::SerialPortError _Error);
    void slotRead();
    void slotWrite(int);
    void slotConnect();

signals:
    void receive(QString);
    void transmit(QString);
    void stateMsg(QString);
    void state(bool);
};

#endif // COMPORT_H
