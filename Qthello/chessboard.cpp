#include "chessboard.h"
#include "hostthread.h"
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QLayout>
#include <QGridLayout>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
static void clean(Points &current, Points lost) {
    sort(current.begin(), current.end());
    sort(lost.begin(), lost.end());
    for (int i = 0, j = 0; j < lost.size(); i++)
        if (current[i] == lost[j]) {
            current[i] = make_pair(SIZE, SIZE);
            j++;
        }
    sort(current.begin(), current.end());
    for (int i = 0; i < lost.size(); i++)
        current.pop_back();
}
static Points eat(const Points &p1, const Points &p2, const Point &p) {
    Points food;
    int chessBoard[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            chessBoard[i][j] = -1;
    for (int i = 0; i < p1.size(); i++)
        chessBoard[p1[i].first][p1[i].second] = 1;
    for (int i = 0; i < p2.size(); i++)
        chessBoard[p2[i].first][p2[i].second] = 2;
    const int x = p.first, y = p.second;
    if (chessBoard[x][y] != -1)
        return food;
    int tmp;
    for (tmp = 1; x-tmp>0 && chessBoard[x-tmp][y] == 2; tmp++);
    if (tmp!=1 && x-tmp>=0 && chessBoard[x-tmp][y] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x-i, y));
    for (tmp = 1; y-tmp>0 && chessBoard[x][y-tmp] == 2; tmp++);
    if (tmp!=1 && y-tmp>=0 && chessBoard[x][y-tmp] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x, y-i));
    for (tmp = 1; x+tmp<SIZE && chessBoard[x+tmp][y] == 2; tmp++);
    if (tmp!=1 && x+tmp<SIZE && chessBoard[x+tmp][y] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x+i, y));
    for (tmp = 1; y+tmp<SIZE && chessBoard[x][y+tmp] == 2; tmp++);
    if (tmp!=1 && y+tmp<SIZE && chessBoard[x][y+tmp] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x, y+i));
    for (tmp = 1; x-tmp>0 && y-tmp>0 && chessBoard[x-tmp][y-tmp] == 2; tmp++);
    if (tmp!=1 && x-tmp>=0 && y-tmp>=0 && chessBoard[x-tmp][y-tmp] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x-i, y-i));
    for (tmp = 1; x-tmp>0 && y+tmp<SIZE && chessBoard[x-tmp][y+tmp] == 2; tmp++);
    if (tmp!=1 && x-tmp>=0 && y+tmp<SIZE && chessBoard[x-tmp][y+tmp] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x-i, y+i));
    for (tmp = 1; x+tmp<SIZE && y-tmp>0 && chessBoard[x+tmp][y-tmp] == 2; tmp++);
    if (tmp !=1 && x+tmp<SIZE && y-tmp>=0 && chessBoard[x+tmp][y-tmp] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x+i, y-i));
    for (tmp = 1; x+tmp<SIZE && y+tmp<SIZE && chessBoard[x+tmp][y+tmp] == 2; tmp++);
    if (tmp !=1 && x+tmp<SIZE && y+tmp<SIZE && chessBoard[x+tmp][y+tmp] == 1)
        for (int i = 1; i < tmp; i++)
            food.push_back(make_pair(x+i, y+i));
    return food;
}
static Points calc(const Points &p1, const Points &p2) {
    Points points;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (eat(p1, p2, make_pair(i, j)).size() != 0)
                points.push_back(make_pair(i, j));
    return points;
}
ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent), showNextStep(false)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            chessmen[i][j] = new QLabel(this);
            gridLayout->addWidget(chessmen[i][j], i, j);
            chessmen[i][j]->installEventFilter(this);
        }
    installEventFilter(this);
}
ChessBoard::~ChessBoard() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            chessmen[i][j]->deleteLater();
}
void ChessBoard::setColor(bool black) {
    colorBlack = black;
}
void ChessBoard::setTurn(bool local) {
    turn = local;
}
void ChessBoard::drawChess() {
    static QPixmap board = QPixmap("board.png");
    static QPixmap black = QPixmap("black.png");
    static QPixmap white = QPixmap("white.png");
    static QPixmap local = QPixmap("local.png");
    static QPixmap remote = QPixmap("remote.png");
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            chessmen[i][j]->setPixmap(board.scaled(chessmen[i][j]->size(), Qt::KeepAspectRatio));
    for (int i = 0; i < localChessmen.size(); i++) {
        QLabel *tmp = chessmen[localChessmen[i].first][localChessmen[i].second];
        tmp->setPixmap((colorBlack ? black : white).scaled(tmp->size(), Qt::KeepAspectRatio));
    }

    for (int i = 0; i < remoteChessmen.size(); i++) {
        QLabel *tmp = chessmen[remoteChessmen[i].first][remoteChessmen[i].second];
        tmp->setPixmap((!colorBlack ? black : white).scaled(tmp->size(), Qt::KeepAspectRatio));
    }

    if (!showNextStep)
        return ;
    const Points points = (turn ? calc(localChessmen, remoteChessmen) : calc(remoteChessmen, localChessmen));
    for (int i = 0; i < points.size(); i++) {
        QLabel *tmp = chessmen[points[i].first][points[i].second];
        tmp->setPixmap((turn ? local : remote).scaled(tmp->size(), Qt::KeepAspectRatio));
    }
}
bool ChessBoard::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == this) {
        if (eve->type() == QEvent::Resize) {
            drawChess();
            return true;
        } else if (eve->type() == QEvent::MouseButtonRelease) {
            showNextStep = false;
            drawChess();
            return true;
        } else if (eve->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent *>(eve)->button() == Qt::RightButton) {
            showNextStep = true;
            drawChess();
            return true;
        }
        return false;
    }
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
    const Points food = eat(localChessmen, remoteChessmen, id);
    if (food.size() == 0)
        return false;
    cerr << food.size() << endl;
    for (int i = 0; i < food.size(); i++)
        cerr << food[i].first << " " << food[i].second << endl;
    clean(remoteChessmen, food);
    for (int i = 0; i < food.size(); i++)
        localChessmen.push_back(food[i]);
    localChessmen.push_back(id);
    drawChess();
    turn = false;
    emit localChess(id.first, id.second);
    return true;
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
    drawChess();
}
void ChessBoard::startHost() {
    hostThread = new HostThread(this);
    hostThread->start();
}
void ChessBoard::endHost() {
    hostThread->exit();
    hostThread->deleteLater();
}
