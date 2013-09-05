#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipconverter.h"
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QSignalMapper>
#include <QGridLayout>
#include <QLayout>
#include <QTcpServer>
static const int PORT = 3141;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), hostServer(NULL), remoteConnect(NULL)
{
    ui->setupUi(this);

    centralWidget()->setLayout(ui->gridLayout);
    chessBoard = new ChessBoard(this);
    ui->chessLayout->addWidget(chessBoard);
    QObject::connect(this, SIGNAL(connectEstablished(int)), chessBoard, SLOT(startGame(int)));
    QObject::connect(chessBoard, SIGNAL(currentRound(int)), this, SLOT(showRoundNumber(int)));
    QObject::connect(chessBoard, SIGNAL(localScore(int)), this, SLOT(showLocalScore(int)));
    QObject::connect(chessBoard, SIGNAL(remoteScore(int)), this, SLOT(showRemoteScore(int)));

    statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createHost_clicked()
{
    ui->connectHost->setDisabled(true);
    ui->createHost->setDisabled(true);

    if (hostServer == NULL) {
        if (remoteConnect != NULL) {
            qDebug() << "terminate Client";
            remoteConnect->deleteLater();
            remoteConnect = NULL;
        }
        qDebug() << "create Host";
        hostServer = new QTcpServer(this);
        hostServer->listen(QHostAddress::Any, PORT);
        QObject::connect(hostServer, SIGNAL(newConnection()), this, SLOT(receiveConnect()));
    }

    QDialog *createHostDialog = new QDialog(this);
    QLabel *label = new QLabel("Host IP: ", createHostDialog);
    QLineEdit *lineEdit = new QLineEdit(createHostDialog);
    lineEdit->setReadOnly(true);
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, AddressList)
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress::Null &&
            address != QHostAddress::LocalHost)
            if (!address.toString().contains("127.0.")){
                lineEdit->setText(address.toString());
                break;
            }
    QPushButton *ok = new QPushButton(createHostDialog);
    ok->setText("keep listening");
    QPushButton *cancel = new QPushButton(createHostDialog);
    cancel->setText("terminate");
    QGridLayout *gridLayout = new QGridLayout(createHostDialog);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 1, 0);
    gridLayout->addWidget(ok, 0, 1);
    gridLayout->addWidget(cancel, 1, 1);

    QObject::connect(ok, SIGNAL(clicked()), createHostDialog, SLOT(close()));
    QObject::connect(cancel, SIGNAL(clicked()), createHostDialog, SLOT(accept()));
    createHostDialog->show();

    if (createHostDialog->exec() == QDialog::Accepted) {
        qDebug() << "terminate Host";
        hostServer->deleteLater();
        hostServer = NULL;
    }

    ui->connectHost->setEnabled(true);
    ui->createHost->setEnabled(true);
}

void MainWindow::on_connectHost_clicked()
{
    ui->connectHost->setDisabled(true);
    ui->createHost->setDisabled(true);

    QDialog *connectHostDialog = new QDialog(this);
    QLabel *label = new QLabel("Host IP: ", connectHostDialog);
    QLineEdit *lineEdit = new QLineEdit(connectHostDialog);
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignRight);
    QGridLayout *gridLayout = new QGridLayout(connectHostDialog);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 0, 1, 1, 4);

    QSignalMapper *mapper = new QSignalMapper(connectHostDialog);
    for (int i = 0; i < 10; i++) {
        QPushButton *p = new QPushButton(QString("%1").arg(i), connectHostDialog);
        gridLayout->addWidget(p, 1+i/5, i%5);
        QObject::connect(p, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(p, i);
    }
    QPushButton *dot = new QPushButton(QString("."), connectHostDialog);
    gridLayout->addWidget(dot, 3, 0);
    QObject::connect(dot, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(dot, -2);
    QPushButton *del = new QPushButton(QString("delete"), connectHostDialog);
    gridLayout->addWidget(del, 3, 1);
    QObject::connect(del, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(del, -1);

    QPushButton *ok = new QPushButton(QString("ok"), connectHostDialog);
    gridLayout->addWidget(ok, 3, 2, 1, 3);
    IPconverter *converter = new IPconverter(connectHostDialog);

    QObject::connect(ok, SIGNAL(clicked()), connectHostDialog, SLOT(accept()));
    QObject::connect(mapper, SIGNAL(mapped(int)), converter, SLOT(addNumber(int)));
    QObject::connect(converter, SIGNAL(changed(QString)), lineEdit, SLOT(setText(QString)));
    connectHostDialog->show();

    if (connectHostDialog->exec() == QDialog::Accepted) {
        if (hostServer != NULL) {
            qDebug() << "terminate Host";
            hostServer->deleteLater();
            hostServer = NULL;
        }
        if (remoteConnect != NULL) {
            qDebug() << "terminate Client";
            remoteConnect->deleteLater();
            remoteConnect = NULL;
        }
        qDebug() << "create Client";
        remoteConnect = new QTcpSocket(this);
        remoteConnect->connectToHost(converter->getIp(), PORT);
        QObject::connect(remoteConnect, SIGNAL(connected()), this, SLOT(achieveConnect()));
    }

    ui->connectHost->setEnabled(true);
    ui->createHost->setEnabled(true);
}
void MainWindow::receiveConnect() {
    qDebug() << "receive client connect";
    remoteConnect = hostServer->nextPendingConnection();
    chessBoard->setRemoteConnect(remoteConnect);
    remoteConnect = NULL;
    emit connectEstablished(1);
}
void MainWindow::achieveConnect() {
    qDebug() << "achieve host connect";
    chessBoard->setRemoteConnect(remoteConnect);
    remoteConnect = NULL;
    emit connectEstablished(2);
}
void MainWindow::showRoundNumber(int roundNum) {
    ui->roundNumber->setText(QString("Current Round: %1").arg(roundNum));
}
void MainWindow::showLocalScore(int score) {
    ui->localScore->setText(QString("Local Score: %1").arg(score));
}
void MainWindow::showRemoteScore(int score) {
    ui->remoteScore->setText(QString("Remote Score: %1").arg(score));
}
