#include "chessboard.h"
#include <QPixmap>
#include <QEvent>
#include <vector>
#include <cmath>
using namespace std;
ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent)
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            chessmen[i][j] = new QLabel(this);
    installEventFilter(this);
}
ChessBoard::~ChessBoard() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            delete chessmen[i][j];
}

void ChessBoard::setColor(bool black) {
    colorBlack = black;
}
void ChessBoard::setTurn(bool local) {
    turn = local;
}
void ChessBoard::paintEvent(QPaintEvent *) {
  /*  for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            chessmen[i][j]->setPixmap(QPixmap("board.png").scaled(chessmen[i][j]->size(), Qt::KeepAspectRatio));
    */
    for (int i = 0; i < localChessmen.size(); i++) {
        QLabel *tmp = chessmen[localChessmen[i].first][localChessmen[i].second];
        tmp->setPixmap(QPixmap(colorBlack ? "black.png" : "white.png").scaled(tmp->size(), Qt::KeepAspectRatio));
    }

    for (int i = 0; i < remoteChessmen.size(); i++) {
        QLabel *tmp = chessmen[remoteChessmen[i].first][remoteChessmen[i].second];
        tmp->setPixmap(QPixmap(!colorBlack ? "black.png" : "white.png").scaled(tmp->size(), Qt::KeepAspectRatio));
    }
    /*
    const Points points = calc();
    for (int i = 0; i < points.size(); i++) {
        QLabel *tmp = chessmen[remoteChessmen[i].first][remoteChessmen[i].second];
        tmp->setPixmap(QPixmap(turn ? "local.png" : "remote.png").scaled(tmp->size(), Qt::KeepAspectRatio));
    }*/
}
bool ChessBoard::eventFilter(QObject *obj, QEvent *eve) {
    if (!turn)
        return false;
    if (eve->type() != QEvent::MouseButtonDblClick)
        return false;
    Point id = make_pair(-1, -1);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (obj == chessmen[i][j])
                id = make_pair(i, j);
    if (id == make_pair(-1, -1))
        return false;
    const Points points = calc();
    for (int i = 0; i < points.size(); i++)
        if (id == points[i]) {
            localChessmen.push_back(id);
            emit localChess(id.first, id.second);
            return true;
        }
}
Points ChessBoard::calc() {
    int chessBoard[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            chessBoard[i][j] = -1;
    for (int i = 0; i < localChessmen.size(); i++)
        chessBoard[localChessmen[i].first][localChessmen[i].second] = 1;
    for (int i = 0; i < remoteChessmen.size(); i++)
        chessBoard[remoteChessmen[i].first][remoteChessmen[i].second] = 0;
    Points points;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (chessBoard[i][j] == -1) {
                if (turn) {
                    int tmp = 1;
                    while (i -tmp > 0 && chessBoard[i - tmp][j] == 0)  tmp++;
                    if (tmp !=1 && i - tmp > 0 && chessBoard[i - tmp - 1][j] == 1) chessBoard[i][j] = 2;
                    tmp = 1;
                    while (j -tmp > 0 && chessBoard[i][j - tmp] == 0)  tmp++;
                    if (tmp !=1 && j - tmp > 0 && chessBoard[i][j - tmp - 1] == 1) chessBoard[i][j] = 2;
                    tmp = 1;
                    while (i +tmp < SIZE && chessBoard[i + tmp][j] == 0)   tmp++;
                    if (tmp !=1 && i + tmp + 1 < SIZE && chessBoard[i + tmp + 1][j] == 1)  chessBoard[i][j] = 2;
                    tmp = 1;
                    while (j +tmp < SIZE && chessBoard[i][j + tmp] == 0)   tmp++;
                    if (tmp !=1 && j + tmp + 1 < SIZE && chessBoard[i][j + tmp + 1] == 1)  chessBoard[i][j] = 2;

                    tmp = 1;
                    while (i-tmp>0 && j-tmp>0 && chessBoard[i-tmp][j-tmp] == 0)  tmp++;
                    if (tmp !=1 && i-tmp>0 && j-tmp>0 && chessBoard[i-tmp-1][j-tmp-1] == 1) chessBoard[i][j] = 2;
                    tmp = 1;
                    while (i-tmp>0 && j+tmp<SIZE && chessBoard[i-tmp][j+tmp] == 0)  tmp++;
                    if (tmp !=1 && i-tmp>0 && j+tmp+1<SIZE && chessBoard[i-tmp-1][j+tmp+1] == 1) chessBoard[i][j] = 2;
                    tmp = 1;
                    while (i+tmp<SIZE && j-tmp>0 && chessBoard[i+tmp][j-tmp] == 0)   tmp++;
                    if (tmp !=1 && i+tmp+1<SIZE && j-tmp>0 && chessBoard[i+tmp+1][j-tmp-1] == 1)  chessBoard[i][j] = 2;
                    tmp = 1;
                    while (i+tmp<SIZE && j+tmp<SIZE && chessBoard[i+tmp][j+tmp] == 0)   tmp++;
                    if (tmp !=1 && i+tmp+1<SIZE && j+tmp+1<SIZE && chessBoard[i+tmp+1][j+tmp+1] == 1)  chessBoard[i][j] = 2;
                }
                if (chessBoard[i][j] == 2)
                    points.push_back(make_pair(i, j));
            }
    return points;
}
void ChessBoard::remoteChess(int x, int y) {
    remoteChessmen.push_back(make_pair(x, y));
}
void ChessBoard::setInit() {
    Points p1, p2;
    p1.push_back(make_pair(SIZE/2-1,SIZE/2-1));
    p1.push_back(make_pair(SIZE/2,SIZE/2));
    p2.push_back(make_pair(SIZE/2-1,SIZE/2));
    p2.push_back(make_pair(SIZE/2,SIZE/2-1));
    if (colorBlack) {
        localChessmen = p1;
        remoteChessmen = p2;
    } else {
        localChessmen = p2;
        remoteChessmen = p1;
    }
}
