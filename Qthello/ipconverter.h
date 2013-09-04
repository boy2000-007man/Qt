#ifndef IPCONVERTER_H
#define IPCONVERTER_H

#include <QObject>
class IPconverter : public QObject
{
    Q_OBJECT
public:
    explicit IPconverter(QObject *parent = 0);
    QString getIp();
private:
    QString ip;
signals:
    void changed(QString);
public slots:
    void addNumber(int k);
};

#endif // IPCONVERTER_H
