#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "chess.h"
#include <QWidget>
#include <QLabel>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();
    void setColor(bool black = true);
    void setTurn(bool local = true);
    void setInit();
private:
    Points localChessmen;
    Points remoteChessmen;
    QLabel *chessmen[SIZE][SIZE];
    void drawChess();
    bool eventFilter(QObject *, QEvent *);
    bool colorBlack;
    bool showNextStep;
    bool turn;
    QTcpServer *hostServer;
    QTcpSocket *clientSocket;
signals:
    void localChess(int, int);
    void result(int);
    void created();
public slots:
    void remoteChess(int, int);
    void createHost();
    void terminateHost();
    void connectRemote();
    void disconnectRemote();
};

#endif // CHESSBOARD_H
