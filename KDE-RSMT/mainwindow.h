#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwidget.h"
#include "mapwidget.h"
#include "sdk.h"
#include "transform.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    DrawWidget *drawWidget;
    MapWidget *mapWidget;
    sdk::Graph *graph;
    sdk::Transform *drawTransform, *mapTransform;
private slots:
    void on_actionOpen_File_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionShow_Map_triggered();

    void on_actionAdd_Point_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
