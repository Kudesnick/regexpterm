#include "socket.h"
#include <QTextStream>

Socket::Socket(QString _Host, int _Port,  QObject *_prnt)
    : QTcpSocket(_prnt)
    , Host(_Host)
    , Port(_Port)
{
    connect(this, &QTcpSocket::connected, this, &Socket::slotConnected);
    connect(this, &QTcpSocket::readyRead, this, &Socket::slotRead);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Socket::slotError);

    tmr = new QTimer();
    QTimer::singleShot(1000, this, &Socket::slotConnect);
};

void Socket::slotConnect()
{
    emit state(false);
    emit stateMsg("Connecting..");
    QTcpSocket::connectToHost(Host, Port);
}

void Socket::send(QString _Str)
{
    QTextStream os(this);
    os << _Str;
    emit transmit(_Str);
}

void Socket::slotConnected()
{
    emit state(true);
    emit stateMsg("The connection success.");
}

void Socket::slotDisconnected()
{
    emit state(false);
    emit stateMsg("Disconnect socket.");

    QTimer::singleShot(6000, this, &Socket::slotConnect);
}

void Socket::slotRead()
{
    /// @todo Данный режим обеспечивает полноценое эхо и диалоги, но затрудняет работу с регулярными выражениями
#if (1)
    QString str = readAll();
    if (str.length())
    {
        emit receive(str);
    }
#else
    while (canReadLine())
    {
        QString str = readLine();
        emit receive(str);
    }
#endif
}

void Socket::slotError(QAbstractSocket::SocketError _Error)
{
    emit state(false);
    QString strError = 
        "Error: " + (_Error == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     _Error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     _Error == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(errorString())
                    );
    emit stateMsg(strError);

    /// @todo add setting autoconnect
    QTimer::singleShot(6000, this, &Socket::slotConnect);
}
