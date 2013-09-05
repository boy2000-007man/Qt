#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void connectEstablished(int);

private slots:
    void on_createHost_clicked();

    void on_connectHost_clicked();

    void receiveConnect();
    void achieveConnect();
    void showRoundNumber(int);
    void showLocalScore(int);
    void showRemoteScore(int);
    void showResult(int);
private:
    Ui::MainWindow *ui;

    ChessBoard *chessBoard;
    QTcpServer *hostServer;
    QTcpSocket *remoteConnect;
};

#endif // MAINWINDOW_H
