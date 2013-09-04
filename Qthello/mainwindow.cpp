#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QGridLayout>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    centralWidget()->setLayout(ui->gridLayout);
    chessBoard = new ChessBoard(this);
    ui->chessLayout->addWidget(chessBoard);

    statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
