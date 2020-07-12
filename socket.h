#ifndef SOCKET_H
#define SOCKET_H

#include <QtNetwork/QTcpSocket>

class Socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Socket(QString _Host, int _Port,  QObject *_prnt = nullptr);
    void send(QString _Str);
protected:
    QString Host;
    int Port;

private:
    void slotConnected();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError _Error);
    void slotRead();

signals:
    void output(QString);
    void state(QString);
};

#endif // SOCKET_H
