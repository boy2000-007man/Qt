#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QTcpSocket>
class Tcp : public QObject
{
    Q_OBJECT
public:
    explicit Tcp(QObject *parent = 0);
    QTcpSocket *tcpSocket;
signals:
    void remoteChess(int, int);
public slots:
    void sendLocalChess(int, int);
    void waitRemoteChess();
};

#endif // TCP_H
