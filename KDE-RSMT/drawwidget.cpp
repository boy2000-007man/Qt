#include "drawwidget.h"
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>
#include <cmath>
using namespace std;
DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent),
    zoom(0.9),
    selectPoint(false)
{
}
void DrawWidget::changeZoom(int k) {
    zoom *= pow(1.1, k);
}
void DrawWidget::paintEvent(QPaintEvent *) {
    if (graph.points().size() == 0)
        return ;

    vector<sdk::PointType> info = graph.information();
    double kx = width() * zoom / max(1.0, info[2]);
    double ky = height() * zoom / max(1.0, info[3]);
    kx = ky = min(kx, ky);
    double dx = (width() - kx * info[2]) / 2;
    double dy = (height() - ky * info[3]) / 2;

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));
    const sdk::Tuples &H = graph.HVs_.first, &V = graph.HVs_.second;
    for (int i = 0; i < H.size(); i++) {
        const sdk::Point p1 = sdk::convert(make_pair(H[i].second.first - info[0], H[i].first - info[1]), kx, ky, dx, dy);
        const sdk::Point p2 = sdk::convert(make_pair(H[i].second.second - info[0], H[i].first - info[1]), kx, ky, dx, dy);
        painter.drawLine(p1.first, p1.second, p2.first, p2.second);
    }
    for (int i = 0; i < V.size(); i++) {
        const sdk::Point p1 = sdk::convert(make_pair(V[i].first - info[0], V[i].second.first - info[1]), kx, ky, dx, dy);
        const sdk::Point p2 = sdk::convert(make_pair(V[i].first - info[0], V[i].second.second - info[1]), kx, ky, dx, dy);
        painter.drawLine(p1.first, p1.second, p2.first, p2.second);
    }
    painter.setPen(QPen(Qt::green, 7));
    for (int i = 0; i < graph.points().size(); i++) {
        const sdk::Point p = sdk::convert(make_pair(graph.points()[i].first - info[0], graph.points()[i].second - info[1]), kx, ky, dx, dy);
        painter.drawPoint(p.first, p.second);
    }
}

bool DrawWidget::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == this)
        switch (eve->type()) {
            case QEvent::MouseMove: {
                if (selectPoint) {
                    QMouseEvent *m = static_cast<QMouseEvent *>(eve);
                    cerr << "detect move" << endl;
                    vector<sdk::PointType> info = graph.information();
                    double kx = width() * zoom / max(1.0, info[2]);
                    double ky = height() * zoom / max(1.0, info[3]);
                    kx = ky = min(kx, ky);
                    double dx = (width() - kx * info[2]) / 2;
                    double dy = (height() - ky * info[3]) / 2;
                    sdk::Point s_p = sdk::reconvert(make_pair(m->x(), m->y()), kx, ky, dx, dy);
                    s_p.first += info[0];
                    s_p.second += info[1];

                    graph.deletepoint(selectedPoint);
                    graph.addpoint(selectedPoint = s_p);
                    setObjectName(QString("Move Point: [%1, %2]").arg(s_p.first).arg(s_p.second));
                }
            }
            return true;
            case QEvent::MouseButtonPress: {
                if (this->graph.points().size() == 0)
                    return false;
                QMouseEvent *m = static_cast<QMouseEvent *>(eve);

                vector<sdk::PointType> info = graph.information();
                double kx = width() * zoom / max(1.0, info[2]);
                double ky = height() * zoom / max(1.0, info[3]);
                kx = ky = min(kx, ky);
                double dx = (width() - kx * info[2]) / 2;
                double dy = (height() - ky * info[3]) / 2;
                sdk::Point s_p = sdk::reconvert(make_pair(m->x(), m->y()), kx, ky, dx, dy);
                s_p.first += info[0];
                s_p.second += info[1];
                int number = 0;
                sdk::Point m_p = make_pair(m->x(), m->y());
                for (int i = 0; i < graph.points().size(); i++) {
                    sdk::Point n_p = sdk::convert(make_pair(graph.points()[i].first - info[0], graph.points()[i].second - info[1]), kx, ky, dx, dy);
                    sdk::Point o_p = sdk::convert(make_pair(graph.points()[number].first - info[0], graph.points()[number].second - info[1]), kx, ky, dx, dy);
                    if (sdk::distance(n_p, m_p) < sdk::distance(o_p, m_p))
                        number = i;
                }
                const sdk::Point c_p = graph.points()[number];
                const sdk::Point rc_p = make_pair(c_p.first-info[0], c_p.second-info[1]);
                const double mc_dist = sdk::distance(m_p, sdk::convert(rc_p, kx, ky, dx, dy));
cerr << mc_dist << endl;
                using namespace sdk;
                cerr << s_p << endl;
                cerr << c_p << endl;
                switch (m->button()) {
                    case Qt::LeftButton:
                        if (mc_dist > 7) {
                            selectPoint = false;
                            graph.addpoint(s_p);
                            setObjectName(QString("Add Point: [%1, %2]").arg(s_p.first).arg(s_p.second));
                        } else {
                            selectPoint = true;
                            selectedPoint = c_p;
                            setObjectName(QString("No Point Add"));
                        }
                        break;
                    case Qt::RightButton: {

                        if (mc_dist < 5) {
                            graph.deletepoint(c_p);
                            setObjectName(QString("Delete Point: [%1, %2]").arg(c_p.first).arg(c_p.second));
                        } else
                            setObjectName(QString("No Point Delete"));
                        break;
                    }
                }
            return true;
            }
            case QEvent::Wheel: {
                QWheelEvent *w = static_cast<QWheelEvent *>(eve);
                if (w->orientation() == Qt::Vertical)
                    changeZoom(w->delta() / 120);
                cerr << w->delta();
                setObjectName(QString("Zoom: %1%").arg(zoom * 100));
            }
            return true;
        }
    return false;
}
