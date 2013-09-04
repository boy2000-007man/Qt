#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "chess.h"
#include "hostthread.h"
#include <QWidget>
#include <QLabel>
class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();
    void startHost();
    void endHost();
    //void setClient();
private:
    HostThread *hostThread;
    void setColor(bool black = true);
    void setTurn(bool local = true);
    void setInit();
    Points localChessmen;
    Points remoteChessmen;
    QLabel *chessmen[SIZE][SIZE];
    void drawChess();
    bool eventFilter(QObject *, QEvent *);
    bool colorBlack;
    bool showNextStep;
    bool turn;
signals:
    void localChess(int, int);
    void result(int);
public slots:
    void remoteChess(int, int);
};

#endif // CHESSBOARD_H
