#include "hostthread.h"
#include "network.h"
#include <QtNetwork/QTcpServer>
HostThread::HostThread(QObject *parent) :
    QThread(parent)
{
}
void HostThread::run() {
    QTcpServer hostServer;
    hostServer.listen(QHostAddress::Any, PORT);
}
