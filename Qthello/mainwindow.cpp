#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QGridLayout>
#include <QLayout>
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
    ok->setText("continue");
    QPushButton *cancel = new QPushButton(dialog);
    cancel->setText("abandon");
    QGridLayout *gridLayout = new QGridLayout(dialog);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 1, 0);
    gridLayout->addWidget(ok, 0, 1);
    gridLayout->addWidget(cancel, 1, 1);
    dialog->show();

    QObject::connect(ok, SIGNAL(clicked()), dialog, SLOT(close()));
    QObject::connect(cancel, SIGNAL(clicked()), dialog, SLOT(close()));
}
