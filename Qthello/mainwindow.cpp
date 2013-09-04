#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipconverter.h"
#include <QLayout>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QGridLayout>
#include <QLayout>
#include <QSignalMapper>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QGridLayout *mainLayout = new QGridLayout(this);
    centralWidget()->setLayout(ui->gridLayout);
    chessBoard = new ChessBoard(this);
    ui->chessLayout->addWidget(chessBoard);
    chessBoard->setColor();
    chessBoard->setTurn();
    chessBoard->setInit();

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

    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel("Host IP: ", dialog);
    QLineEdit *lineEdit = new QLineEdit(dialog);
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
    QPushButton *ok = new QPushButton(dialog);
    ok->setText("keep listening");
    QPushButton *cancel = new QPushButton(dialog);
    cancel->setText("terminate");
    QGridLayout *gridLayout = new QGridLayout(dialog);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 1, 0);
    gridLayout->addWidget(ok, 0, 1);
    gridLayout->addWidget(cancel, 1, 1);
    dialog->show();

    QObject::connect(ok, SIGNAL(clicked()), dialog, SLOT(close()));
    QObject::connect(cancel, SIGNAL(clicked()), dialog, SLOT(close()));
    QObject::connect(cancel, SIGNAL(clicked()), chessBoard, SLOT(terminateHost()));
}

void MainWindow::on_connectHost_clicked()
{
    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel("Host IP: ", dialog);
    QLineEdit *lineEdit = new QLineEdit(dialog);
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignRight);
    QGridLayout *gridLayout = new QGridLayout(dialog);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 0, 1, 1, 4);

    QSignalMapper *mapper = new QSignalMapper(dialog);
    for (int i = 0; i < 10; i++) {
        QPushButton *p = new QPushButton(QString("%1").arg(i), dialog);
        gridLayout->addWidget(p, 1+i/5, i%5);
        QObject::connect(p, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(p, i);
    }
    QPushButton *dot = new QPushButton(QString("."), dialog);
    gridLayout->addWidget(dot, 3, 0);
    QObject::connect(dot, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(dot, -2);
    QPushButton *del = new QPushButton(QString("delete"), dialog);
    gridLayout->addWidget(del, 3, 1);
    QObject::connect(del, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(del, -1);

    QPushButton *ok = new QPushButton(QString("ok"), dialog);
    gridLayout->addWidget(ok, 3, 2, 1, 3);
    QObject::connect(ok, SIGNAL(clicked()), dialog, SLOT(close()));
    IPconverter *converter = new IPconverter(this);
    QObject::connect(mapper, SIGNAL(mapped(int)), converter, SLOT(addNumber(int)));
    QObject::connect(converter, SIGNAL(changed(QString)), lineEdit, SLOT(setText(QString)));

    dialog->show();
}
