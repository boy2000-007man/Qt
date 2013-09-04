#ifndef HOSTTHREAD_H
#define HOSTTHREAD_H

#include <QThread>

class HostThread : public QThread
{
    Q_OBJECT
public:
    explicit HostThread(QObject *parent = 0);
private:
    void run();
signals:
    
public slots:
    
};

#endif // HOSTTHREAD_H
