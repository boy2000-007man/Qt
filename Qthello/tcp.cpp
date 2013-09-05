#include "tcp.h"
#include <QDataStream>
Tcp::Tcp(QObject *parent) :
    QObject(parent)
{
}
void Tcp::sendLocalChess(int x, int y) {
    QDataStream out(tcpSocket);
    out << x << y;
    qDebug() << "send message";
    emit waitRemoteChess();
}
void Tcp::waitRemoteChess() {
    while (!tcpSocket->waitForReadyRead());
    QDataStream in(tcpSocket);
    int x, y;
    in >> x >> y;
    qDebug() << "receive message";
    emit remoteChess(x, y);
}
