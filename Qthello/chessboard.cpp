#include "chessboard.h"
#include "gameplatform.h"
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QLayout>
#include <QGridLayout>
#include <vector>
#include <cmath>
#include <algorithm>
#include <QDebug>
#include <QThread>
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
    QWidget(parent), remoteConnect(NULL), remoteThread(NULL) {
    QGridLayout *gridLayout = new QGridLayout(this);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            chessmen[i][j] = new QLabel(this);
            gridLayout->addWidget(chessmen[i][j], i, j);
            chessmen[i][j]->installEventFilter(this);
        }
    installEventFilter(this);
    QObject::connect(this, SIGNAL(currentRound(int)), this, SLOT(check()));
}
void ChessBoard::showChess(bool showNextStep) {
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
        tmp->setPixmap((playerNumber == 1 ? white : black).scaled(tmp->size(), Qt::KeepAspectRatio));
    }

    for (int i = 0; i < remoteChessmen.size(); i++) {
        QLabel *tmp = chessmen[remoteChessmen[i].first][remoteChessmen[i].second];
        tmp->setPixmap((playerNumber == 1 ? black : white).scaled(tmp->size(), Qt::KeepAspectRatio));
    }

    if (!showNextStep)
        return ;
    const Points points = (roundNumber%2 == playerNumber-1 ? calc(localChessmen, remoteChessmen) : calc(remoteChessmen, localChessmen));
    for (int i = 0; i < points.size(); i++) {
        QLabel *tmp = chessmen[points[i].first][points[i].second];
        tmp->setPixmap((roundNumber%2 == playerNumber-1 ? local : remote).scaled(tmp->size(), Qt::KeepAspectRatio));
    }
}
bool ChessBoard::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == this) {
        if (eve->type() == QEvent::Resize) {
            showChess();
            return true;
        } else if (eve->type() == QEvent::MouseButtonRelease) {
            showChess();
            return true;
        } else if (eve->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent *>(eve)->button() == Qt::RightButton) {
            showChess(true);
            return true;
        }
        return false;
    }
    if (eve->type() != QEvent::MouseButtonDblClick)
        return false;
    if (roundNumber % 2 != playerNumber - 1)
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
    clean(remoteChessmen, food);
    for (int i = 0; i < food.size(); i++)
        localChessmen.push_back(food[i]);
    localChessmen.push_back(id);

    showChess();
    emit currentRound(++roundNumber);
    emit localScore(localChessmen.size());
    emit remoteScore(remoteChessmen.size());
    emit localChess(id.first, id.second);

    return true;
}
void ChessBoard::remoteChess(int x, int y) {
    Points food = eat(remoteChessmen, localChessmen, make_pair(x, y));
    clean(localChessmen, food);
    while (food.size() != 0) {
        remoteChessmen.push_back(food.back());
        food.pop_back();
    }
    remoteChessmen.push_back(make_pair(x, y));

    showChess();
    emit currentRound(++roundNumber);
    emit localScore(localChessmen.size());
    emit remoteScore(remoteChessmen.size());

}
void ChessBoard::startGame(int playerNum) {
    playerNumber = playerNum;
    Points p1, p2;
    p1.push_back(make_pair(SIZE/2-1,SIZE/2-1));
    p1.push_back(make_pair(SIZE/2,SIZE/2));
    p2.push_back(make_pair(SIZE/2-1,SIZE/2));
    p2.push_back(make_pair(SIZE/2,SIZE/2-1));
    if (playerNumber == 1) {
        localChessmen = p1;
        remoteChessmen = p2;
    } else {
        localChessmen = p2;
        remoteChessmen = p1;
    }

    qDebug() << "player :" << playerNumber << "now game start";

    GamePlatform *gamePlatform = new GamePlatform(remoteConnect);
    remoteConnect = NULL;
    remoteThread = new QThread();
    gamePlatform->moveToThread(remoteThread);
    QObject::connect(this, SIGNAL(localChess(int,int)), gamePlatform, SLOT(sendLocalChess(int,int)));
    QObject::connect(gamePlatform, SIGNAL(remoteChess(int,int)), this, SLOT(remoteChess(int,int)));
    QObject::connect(gamePlatform, SIGNAL(sendComplete()), gamePlatform, SLOT(waitRemoteChess()));
    QObject::connect(this, SIGNAL(waitRemoteChess()), gamePlatform, SLOT(waitRemoteChess()));
    remoteThread->start();

    if (playerNumber != 1)
        emit waitRemoteChess();

    showChess();

    emit currentRound(roundNumber = 0);
    emit localScore(localChessmen.size());
    emit remoteScore(remoteChessmen.size());
}
void ChessBoard::setRemoteConnect(QTcpSocket *remoteConnection) {
    remoteConnect = remoteConnection;
}
void ChessBoard::terminateGame() {
    qDebug() << "exit remote thread";
    remoteThread->exit();
    remoteThread->wait();
    remoteThread = NULL;
    remoteConnect->deleteLater();
    remoteConnect = NULL;
    if (localChessmen.size() == remoteChessmen.size())
        emit gameResult(0);
    else
        emit gameResult(localChessmen.size() > remoteChessmen.size() ? 1 : 2);
}
void ChessBoard::check() {
    if (localChessmen.size() + remoteChessmen.size() == SIZE*SIZE)
        terminateGame();
    else if (calc(localChessmen, remoteChessmen).size() == 0)
        terminateGame();
    else if (calc(remoteChessmen, localChessmen).size() == 0)
        terminateGame();
}
