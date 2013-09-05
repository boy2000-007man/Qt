#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipconverter.h"
#include "ipdialog.h"
#include <QLayout>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QGridLayout>
#include <QLayout>
#include <QSignalMapper>
#include "PORT.h"
#include "createhostdialog.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QGridLayout *mainLayout = new QGridLayout(this);
    centralWidget()->setLayout(ui->gridLayout);
    chessBoard = new ChessBoard(this);
    ui->chessLayout->addWidget(chessBoard);

    statusBar()->setSizeGripEnabled(false);
    QObject::connect(ui->createHost, SIGNAL(clicked()), chessBoard, SLOT(createHost()));
    QObject::connect(ui->connectHost, SIGNAL(clicked()), chessBoard, SLOT(terminateHost()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createHost_clicked()
{
    ui->connectHost->setDisabled(true);

    createHostDialog *createHost = new createHostDialog(this);
    createHost->show();

    chessBoard->createHost();
    if (createHost->exec() == QDialog::Rejected)
         chessBoard->terminateHost();
    ui->connectHost->setDisabled(false);
}

void MainWindow::on_connectHost_clicked()
{
    ui->createHost->setDisabled(true);
    IPDialog *ipdialog = new IPDialog(this);
    ipdialog->show();
    if (ipdialog->exec() == QDialog::Accepted) {
        if (chessBoard->remoteSocket != NULL)
            return ;
        chessBoard->remoteSocket = new QTcpSocket(chessBoard);
        chessBoard->remoteSocket->connectToHost(ipdialog->converter->getIp(), PORT);
        QObject::connect(chessBoard->remoteSocket, SIGNAL(connected()), chessBoard, SIGNAL(connectEstablished()));
    }
    ui->createHost->setDisabled(false);
}
