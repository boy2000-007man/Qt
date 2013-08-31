#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include "sdk.h"
#include "transform.h"
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
using namespace sdk;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(800, 600);
    setWindowTitle(name);
    graph = new Graph;
    drawWidget = new DrawWidget(this, graph);
    mapWidget = new MapWidget(0, graph);
    drawTransform = new Transform(drawWidget, graph);
    mapTransform = new Transform(mapWidget, graph);
    drawWidget->transform = drawTransform;
    mapWidget->transform = mapTransform;
    mapWidget->setMaximumSize(200, 200);
    mapWidget->setLineSize(0.0);
    mapWidget->setPointSize(4.0);
    //mapWidget->show();
    setCentralWidget(drawWidget);
    mapWidget->installEventFilter(mapWidget);
    centralWidget()->installEventFilter(centralWidget());
    QObject::connect(mapWidget, SIGNAL(clickPoint(double, double)), drawWidget, SLOT(targetTo(double, double)));
    QObject::connect(centralWidget(), SIGNAL(objectNameChanged(QString)), statusBar(), SLOT(showMessage(QString)));
    QObject::connect(centralWidget(), SIGNAL(objectNameChanged(QString)), centralWidget(), SLOT(update()));
    QObject::connect(centralWidget(), SIGNAL(objectNameChanged(QString)), mapWidget, SLOT(update()));
    statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete drawWidget;
    delete mapWidget;
    delete graph;
}

void MainWindow::on_actionOpen_File_triggered()
{
    QFileDialog *open_file = new QFileDialog(this);
    const string filename = open_file->getOpenFileName().toStdString();
    if (filename == "")
        return ;
    ifstream fin(filename.c_str());

    statusBar()->showMessage(QString("Open File: %1").arg(filename.c_str()));
    if (!fin)
        QMessageBox::warning(this, tr("File Open Error!"), tr("An error occur when open selected file."), QMessageBox::Abort);
    else {
        cerr << "Reading ..." << endl;
        fin >> *(drawWidget->graph);
        fin.close();
        statusBar()->showMessage(QString("File: %1 Open Complete").arg(filename.c_str()));
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

void MainWindow::on_actionShow_Map_triggered()
{
    mapWidget->show();
}
