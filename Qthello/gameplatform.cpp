#include "gameplatform.h"
#include <QDataStream>
GamePlatform::GamePlatform(QTcpSocket *remoteConnection, QObject *parent) :
    QObject(parent), remoteConnect(remoteConnection)
{
    remoteConnect->setParent(this);
}
void GamePlatform::sendLocalChess(int x, int y) {
    qDebug() << "send local chess" << x << " " << y;
    QDataStream out(remoteConnect);
    out << x << y;
    emit sendComplete();
}
void GamePlatform::waitRemoteChess() {
    qDebug() << "wait for remote chess";
    while (!remoteConnect->waitForReadyRead());
    QDataStream in(remoteConnect);
    int x, y;
    in >> x >> y;
    qDebug() << "read remote chess" << x << " " << y;
    emit remoteChess(x, y);
}
