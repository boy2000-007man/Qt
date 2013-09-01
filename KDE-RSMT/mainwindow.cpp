#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include "sdk.h"
#include "dialog.h"
#include "transform.h"
#include <QObject>
#include <QFileDialog>
#include <QtGui>
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QInputDialog>
#include <cmath>
using namespace std;
using namespace sdk;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(name);
    setMinimumSize(800, 600);
    graph = new Graph;
    drawWidget = new DrawWidget(this, graph);
    mapWidget = new MapWidget(drawWidget, graph);
    drawTransform = new Transform(drawWidget, graph);
    mapTransform = new Transform(mapWidget, graph);
    drawWidget->transform = drawTransform;
    mapWidget->transform = mapTransform;

    mapWidget->setDrawWidget(drawWidget);
    mapWidget->setFixedSize(150, 150);
    mapWidget->setLineSize(0.0);
    mapWidget->setPointSize(3.0);
    mapWidget->hide();
    mapWidget->installEventFilter(mapWidget);

    setCentralWidget(drawWidget);
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
    delete graph;
    delete drawTransform;
    delete mapTransform;
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return ;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::warning(this, tr("No File Opened!"), tr("Please check your input."), QMessageBox::Abort);
    else {
        QTextStream fin(&file);
        Points points;
        int num;
        fin >> num;
        for (int i = 0; i < num; i++) {
            Point tmp;
            fin >> tmp.first >> tmp.second;
            points.push_back(tmp);
        }
        file.close();
        graph->setPoints(points);
        drawWidget->transform->setFocusPolicy();
        statusBar()->showMessage(QString("Open File: %1").arg(fileName));
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
    if (mapWidget->isHidden()) {
        mapWidget->show();
        ui->actionShow_Map->setText(QString("Hide Map"));
    } else {
        mapWidget->hide();
        ui->actionShow_Map->setText(QString("Show Map"));
    }
}

void MainWindow::on_actionAdd_Point_triggered()
{
    Dialog *dialog = new Dialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        graph->addPoint(make_pair(dialog->getX(), dialog->getY()));
        drawWidget->setObjectName(QString("Input Point: [%1, %2]").arg(dialog->getX()).arg(dialog->getY()));
    }
    delete dialog;
}
