#include "comport.h"
#include <QTimer>

Comport::Comport(QString _name, enum BaudRate _baud, enum DataBits _dataBits, enum Parity _parity, enum StopBits _stopBits, QObject *_prnt) : QSerialPort(_prnt)
{
    setPortName(_name);
    setBaudRate(_baud);
    setDataBits(_dataBits);
    setParity(_parity);
    setStopBits(_stopBits);
    setFlowControl(QSerialPort::NoFlowControl);

    connect(this, &QSerialPort::readyRead    , this, &Comport::slotRead);
    connect(this, &QSerialPort::bytesWritten , this, &Comport::slotWrite);
    connect(this, &QSerialPort::errorOccurred, this, &Comport::slotError);

    QTimer::singleShot(1000, this, &Comport::slotConnect);
};

void Comport::send(QString _Str)
{
    int len = write(_Str.toUtf8());
    emit transmit(_Str.left(len));
}

void Comport::slotConnect()
{
    open(QSerialPort::ReadWrite);
}

void Comport::slotDisconnected()
{
    emit state(false);
    emit stateMsg("Disconnect socket.");

    QTimer::singleShot(6000, this, &Comport::slotConnect);
}

void Comport::slotRead()
{
    QString str = readAll();
    if (str.length())
    {
        emit receive(str);
    }
}

void Comport::slotWrite(int _len)
{
    (void)_len;
    /// @todo добавить полноценный контроль передачи
}

void Comport::slotError(QSerialPort::SerialPortError _Error)
{
    QString result = "";
    switch (_Error)
    {
        case QSerialPort::NoError:
        {
            emit state(true);
            emit stateMsg("The connection success.");
        }
        return;

        case QSerialPort::FramingError             : result += "FramingError             "; break;
        case QSerialPort::BreakConditionError      : result += "BreakConditionError      "; break;
        case QSerialPort::ParityError              : result += "ParityError              "; break;
        case QSerialPort::DeviceNotFoundError      : result += "DeviceNotFoundError      "; break;
        case QSerialPort::PermissionError          : result += "PermissionError          "; break;
        case QSerialPort::OpenError                : result += "OpenError                "; break;
        case QSerialPort::NotOpenError             : result += "NotOpenError             "; break;
        case QSerialPort::WriteError               : result += "WriteError               "; break;
        case QSerialPort::ReadError                : result += "ReadError                "; break;
        case QSerialPort::ResourceError            : result += "ResourceError            "; break;
        case QSerialPort::UnsupportedOperationError: result += "UnsupportedOperationError"; break;
        case QSerialPort::TimeoutError             : result += "TimeoutError             "; break;
        case QSerialPort::UnknownError             : result += "UnknownError             "; break;
    }

    emit state(false);
    emit stateMsg("Error: " + result);

    /// @todo add setting autoconnect
    QTimer::singleShot(6000, this, &Comport::slotConnect);
}
