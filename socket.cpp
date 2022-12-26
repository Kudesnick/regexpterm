#include "socket.h"

Socket::Socket(QString _Host, int _Port,  QObject *_prnt)
    : QTcpSocket(_prnt)
    , Host_(_Host)
    , Port_(_Port)
{   
    connect(this, &QTcpSocket::connected, this, &Socket::slotConnected);
    connect(this, &QTcpSocket::readyRead, this, &Socket::slotRead);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Socket::slotError);

    QTimer::singleShot(1000, this, &Socket::slotConnect);
};

void Socket::send(QString _Str)
{
    int len = write(_Str.toUtf8());
    emit transmit(_Str.left(len));
}

void Socket::slotConnect()
{
    emit state(false);
    emit stateMsg("Connecting..");
    QTcpSocket::connectToHost(Host, Port);
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
    QString str = readAll();
    if (str.length())
    {
        emit receive(str);
    }
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
