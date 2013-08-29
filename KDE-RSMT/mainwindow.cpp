#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <string>
#include <fstream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    drawwidget(new Drawwidget(this))
{
    ui->setupUi(this);
    setCentralWidget(drawwidget);
    centralWidget()->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == centralWidget())
        if (eve->type() == QEvent::MouseButtonDblClick) {
        }
    return false;
}

void MainWindow::on_actionOpen_File_triggered()
{
    QFileDialog *open_file = new QFileDialog(this);
    const string filename = open_file->getOpenFileName().toStdString();
    if (filename == "")
        return ;
    ifstream fin(filename.c_str());
    cerr << "Open File: " << filename << endl;
    if (!fin)
        QMessageBox::warning(this, tr("File Open Error!"), tr("An error occur when open selected file."), QMessageBox::Abort);
    else {
        cerr << "Reading ..." << endl;
        fin >> drawwidget->graph;
        drawwidget->paintEvent(0);
        this->drawwidget->update();
        //centralWidget()->update();
        using namespace sdk;
        for (int i = 0; i < drawwidget->graph.points().size(); i++)
            cerr << "Add Point: " << drawwidget->graph.points()[i] << endl;
        statusBar()->showMessage(QString("Points' number: %1").arg(QString::number(drawwidget->graph.points().size())));
        //cerr << drawwidget->graph << endl;
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}
