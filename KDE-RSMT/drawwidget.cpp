#include "drawwidget.h"
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>
#include <cmath>
using namespace std;
Drawwidget::Drawwidget(QWidget *parent) :
    QWidget(parent)
{
}
void Drawwidget::paintEvent(QPaintEvent *) {
    if (graph.points().size() == 0)
        return ;
    QPainter p(this);
    int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;
    for (int i = 0; i < graph.points().size(); i++) {
        minx = min(minx, graph.points()[i].first);
        miny = min(miny, graph.points()[i].second);
        maxx = max(maxx, graph.points()[i].first);
        maxy = max(maxy, graph.points()[i].second);
    }
    int x = maxx - minx, y = maxy - miny;
    const double k = 0.1;
    double kx = width() * (1 - k) / max(1, x);
    double ky = height() * (1 - k) / max(1, y);
    kx = ky = min(kx, ky);
    const int dx = (width() - kx * x) / 2;
    const int dy = (height() - ky * y) / 2;
    const sdk::Tuples &H = graph.HVs_.first, &V = graph.HVs_.second;
    p.setPen(QPen(Qt::black, 3));
    for (int i = 0; i < H.size(); i++)
        p.drawLine(dx + (H[i].second.first-minx) * kx, dy + (H[i].first-miny) * ky, dx + (H[i].second.second-minx) * kx, dy + (H[i].first-miny) * ky);
    for (int i = 0; i < V.size(); i++)
        p.drawLine(dx + (V[i].first-minx) * kx, dy + (V[i].second.first-miny) * ky, dx + (V[i].first-minx) * kx, dy + (V[i].second.second-miny) * ky);
    p.setPen(QPen(Qt::green, 7));
    for (int i = 0; i < graph.points().size(); i++)
        p.drawPoint(dx + (graph.points()[i].first-minx) * kx, dy + (graph.points()[i].second-miny) * ky);
}

bool Drawwidget::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == this)
        switch (eve->type()) {
            case QEvent::MouseButtonPress: {
                if (this->graph.points().size() == 0)
                    return false;
                QMouseEvent *m = static_cast<QMouseEvent *>(eve);
                int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;
                for (int i = 0; i < graph.points().size(); i++) {
                    minx = min(minx, graph.points()[i].first);
                    miny = min(miny, graph.points()[i].second);
                    maxx = max(maxx, graph.points()[i].first);
                    maxy = max(maxy, graph.points()[i].second);
                }
                int x = maxx - minx, y = maxy - miny;
                const double k = 0.1;
                double kx = this->width() * (1 - k) / max(1, x);
                double ky = this->height() * (1 - k) / max(1, y);
                kx = ky = min(kx, ky);
                const int dx = (this->width() - kx * x) / 2;
                const int dy = (this->height() - ky * y) / 2;
                const sdk::Point p = make_pair(round((m->x()-dx)/kx+minx), round((m->y()-dy)/ky+miny));
                switch (m->button()) {
                    case Qt::LeftButton:
                        if (graph.addpoint(p))
                            setObjectName(QString("Add point: [%1, %2]").arg(p.first).arg(p.second));
                            //window()->statusBar()->showMessage();
                        break;
                    case Qt::RightButton:
                        if (this->graph.deletepoint(p))
                            setObjectName(QString("Delete point: [%1, %2]").arg(p.first).arg(p.second));
                            //window()->statusBar()->showMessage(QString("Delete point: [%1, %2]").arg(p.first).arg(p.second));
                        break;
                }
                //this->update();
                //statusBar()->showMessage(QString("Points' number: %1").arg(QString::number(this->graph.points().size())));
                //cerr << "Add Point:" << this->graph.points().back() << endl;
                return true;
            }
        }
    return false;
}
