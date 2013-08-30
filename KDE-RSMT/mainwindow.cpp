#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
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
    centralWidget()->installEventFilter(this);
    statusBar()->setSizeGripEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == centralWidget())
        switch (eve->type()) {
            case QEvent::MouseButtonPress: {
                if (drawwidget->graph.points().size() == 0)
                    return false;
                QMouseEvent *m = static_cast<QMouseEvent *>(eve);
                int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;
                for (int i = 0; i < drawwidget->graph.points().size(); i++) {
                    minx = min(minx, drawwidget->graph.points()[i].first);
                    miny = min(miny, drawwidget->graph.points()[i].second);
                    maxx = max(maxx, drawwidget->graph.points()[i].first);
                    maxy = max(maxy, drawwidget->graph.points()[i].second);
                }
                int x = maxx - minx, y = maxy - miny;
                const double k = 0.1;
                double kx = drawwidget->width() * (1 - k) / max(1, x);
                double ky = drawwidget->height() * (1 - k) / max(1, y);
                kx = ky = min(kx, ky);
                const int dx = (drawwidget->width() - kx * x) / 2;
                const int dy = (drawwidget->height() - ky * y) / 2;
                const sdk::Point p = make_pair(round((m->x()-dx)/kx+minx), round((m->y()-dy)/ky+miny));
                switch (m->button()) {
                    case Qt::LeftButton:
                        if (drawwidget->graph.addpoint(p))
                            statusBar()->showMessage(QString("Add point: [%1, %2]").arg(p.first).arg(p.second));
                        break;
                    case Qt::RightButton:
                        if (drawwidget->graph.deletepoint(p))
                            statusBar()->showMessage(QString("Delete point: [%1, %2]").arg(p.first).arg(p.second));
                        break;
                }
                centralWidget()->update();
                //statusBar()->showMessage(QString("Points' number: %1").arg(QString::number(drawwidget->graph.points().size())));
                //cerr << "Add Point:" << drawwidget->graph.points().back() << endl;
                return true;
            }
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
        centralWidget()->update();
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

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "version", version);
}
