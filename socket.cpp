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
    emit state("Connecting..");
    QTcpSocket::connectToHost(Host, Port);
};

void Socket::send(QString _Str)
{
    QTextStream os(this);
    os << _Str << "\n";
}

void Socket::slotConnected()
{
    emit state("The connection success.");
}

void Socket::slotDisconnected()
{
    emit state("Disconnect socket.");
}

void Socket::slotRead()
{
    static QString strRead = "";

    while (canReadLine())
    {
        QString strRead = readLine();

        emit output(strRead);
    }
}

void Socket::slotError(QAbstractSocket::SocketError _Error)
{
    QString strError = 
        "Error: " + (_Error == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     _Error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     _Error == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(errorString())
                    );
    emit state(strError);
}

