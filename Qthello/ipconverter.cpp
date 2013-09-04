#include "ipconverter.h"

IPconverter::IPconverter(QObject *parent) :
    QObject(parent)
{
}
QString IPconverter::getIp() {
    return ip;
}
void IPconverter::addNumber(int k) {
    if (k == -1) {
        ip.remove(ip.length() - 1, 1);
    } else if (k == -2) {
        ip += ".";
    } else {
        ip += char('0' + k);
    }
    emit changed(ip);
}
