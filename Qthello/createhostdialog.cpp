#include "createhostdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QtNetwork/QTcpSocket>
#include <QGridLayout>
#include <QPushButton>
#include <QNetworkInterface>
createHostDialog::createHostDialog(QWidget *parent) :
    QDialog(parent)
{
    QLabel *label = new QLabel("Host IP: ", this);
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, AddressList)
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress::Null &&
            address != QHostAddress::LocalHost)
            if (!address.toString().contains("127.0.")){
                lineEdit->setText(hostIp = address.toString());
                break;
            }
    QPushButton *ok = new QPushButton(this);
    ok->setText("keep listening");
    QPushButton *cancel = new QPushButton(this);
    cancel->setText("terminate");
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 1, 0);
    gridLayout->addWidget(ok, 0, 1);
    gridLayout->addWidget(cancel, 1, 1);

    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}
