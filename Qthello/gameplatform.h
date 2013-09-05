#ifndef GAMEPLATFORM_H
#define GAMEPLATFORM_H

#include <QObject>
#include <QTcpSocket>
class GamePlatform : public QObject
{
    Q_OBJECT
public:
    explicit GamePlatform(QTcpSocket *remoteConnection, QObject *parent = 0);
    
private:
    QTcpSocket *remoteConnect;
signals:
    void remoteChess(int, int);
    void sendComplete();
public slots:
    void sendLocalChess(int, int);
    void waitRemoteChess();
};

#endif // GAMEPLATFORM_H
