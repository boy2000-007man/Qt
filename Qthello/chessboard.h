#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "chess.h"
#include <QWidget>
#include <QLabel>
#include <QTcpSocket>
class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);

    void setRemoteConnect(QTcpSocket *remoteConnection);
private:
    bool eventFilter(QObject *, QEvent *);

    void showChess(bool showNextStep = false);
    QTcpSocket *remoteConnect;
    Points localChessmen;
    Points remoteChessmen;
    QLabel *chessmen[SIZE][SIZE];
    int playerNumber;
    int roundNumber;
    QThread *remoteThread;
signals:
    void localChess(int, int);
    void gameResult(int);
    void currentRound(int);
    void localScore(int);
    void remoteScore(int);
    void waitRemoteChess();
public slots:
    void check();
    void startGame(int);
    void terminateGame();
    void remoteChess(int, int);
};

#endif // CHESSBOARD_H
