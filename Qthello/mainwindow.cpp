#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QGridLayout>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QGridLayout *mainLayout = new QGridLayout(this);
    centralWidget()->setLayout(ui->gridLayout);
    chessBoard = new ChessBoard(this);
    chessBoard->setColor();
    chessBoard->setTurn();
    chessBoard->setInit();
    ui->chessWidget = chessBoard;
    statusBar()->setSizeGripEnabled(false);
    //setLayout(mainLayout);
    //ui->
    //layout()->addItem(ui->buttonsLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
