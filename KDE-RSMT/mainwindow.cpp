#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include <QObject>
#include <QFileDialog>
#include <QtGui>
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    drawwidget(new Drawwidget(this))
{
    ui->setupUi(this);
    setWindowTitle(name);
    setCentralWidget(drawwidget);
    centralWidget()->installEventFilter(centralWidget());
    QObject::connect(centralWidget(), SIGNAL(objectNameChanged(QString)), statusBar(), SLOT(showMessage(QString)));
    QObject::connect(centralWidget(), SIGNAL(objectNameChanged(QString)), centralWidget(), SLOT(update()));
    statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
    QFileDialog *open_file = new QFileDialog(this);
    const string filename = open_file->getOpenFileName().toStdString();
    if (filename == "")
        return ;
    ifstream fin(filename.c_str());
    cerr << "Open File: " << filename << endl;
    statusBar()->showMessage(QString("Open File: %1").arg(filename.c_str()));
    if (!fin)
        QMessageBox::warning(this, tr("File Open Error!"), tr("An error occur when open selected file."), QMessageBox::Abort);
    else {
        cerr << "Reading ..." << endl;
        fin >> drawwidget->graph;
        centralWidget()->update();
        using namespace sdk;
        for (int i = 0; i < drawwidget->graph.points().size(); i++)
            cerr << "Add Point: " << drawwidget->graph.points()[i] << endl;
        //cerr << drawwidget->graph << endl;
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "version", version);
}
