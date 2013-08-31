#include "drawwidget.h"
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>
#include <cmath>
using namespace std;
using namespace sdk;
DrawWidget::DrawWidget(QWidget *parent, Graph *g, Transform *t) :
    QWidget(parent),
    graph(g),
    transform(t),
    selectPoint(false) {
}
void DrawWidget::paintEvent(QPaintEvent *) {
    if (graph->points().size() == 0)
        return ;

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));
    const Tuples &H = graph->HVs_.first, &V = graph->HVs_.second;
    for (int i = 0; i < H.size(); i++) {
        const Point p1 = transform->toScreen(make_pair(H[i].second.first, H[i].first));
        const Point p2 = transform->toScreen(make_pair(H[i].second.second, H[i].first));
        painter.drawLine(p1.first, p1.second, p2.first, p2.second);
    }
    for (int i = 0; i < V.size(); i++) {
        const Point p1 = transform->toScreen(make_pair(V[i].first, V[i].second.first));
        const Point p2 = transform->toScreen(make_pair(V[i].first, V[i].second.second));
        painter.drawLine(p1.first, p1.second, p2.first, p2.second);
    }

    painter.setPen(QPen(Qt::green, 7));
    for (int i = 0; i < graph->points().size(); i++) {
        const Point p = transform->toScreen(graph->points()[i]);
        painter.drawPoint(p.first, p.second);
    }
}
bool DrawWidget::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == this)
        if (graph->points().size() == 0)
            return false;
        else if (eve->type() == QEvent::MouseMove) {
            if (selectPoint) {
                QMouseEvent *m = static_cast<QMouseEvent *>(eve);

                //cerr << "detect move" << endl;
                Point cursorGraphPoint = transform->toGraph(make_pair(m->x(), m->y()));

                graph->deletePoint(pointSelected);
                graph->addPoint(pointSelected = cursorGraphPoint);
                setObjectName(QString("Move Point: [%1, %2]").arg(cursorGraphPoint.first).arg(cursorGraphPoint.second));
                return true;
            }
        } else if (eve->type() == QEvent::MouseButtonPress) {
            QMouseEvent *m = static_cast<QMouseEvent *>(eve);

            Point cursorScreenPoint = make_pair(m->x(), m->y());
            Point cursorGraphPoint = transform->toGraph(cursorScreenPoint);
            Point nearestGraphPoint = graph->searchNearestPoint(cursorGraphPoint);
            Point nearestScreenPoint = transform->toScreen(nearestGraphPoint);
            //PointType shortestGraphDistance = sdk::distance(cursorGraphPoint, nearestGraphPoint);
            PointType shortestScreenDistance = sdk::distance(cursorScreenPoint, nearestScreenPoint);

            selectPoint = false;
            switch (m->button()) {
                case Qt::LeftButton:
                    if (shortestScreenDistance > 7) {
                        graph->addPoint(cursorGraphPoint);
                        setObjectName(QString("Add Point: [%1, %2]").arg(cursorGraphPoint.first).arg(cursorGraphPoint.second));
                    } else {
                        selectPoint = true;
                        pointSelected = nearestGraphPoint;
                        setObjectName(QString("Select Point: [%1, %2]").arg(pointSelected.first).arg(pointSelected.second));
                    }
                    break;
                case Qt::RightButton:
                    if (shortestScreenDistance < 5) {
                        graph->deletePoint(nearestGraphPoint);
                        setObjectName(QString("Delete Point: [%1, %2]").arg(nearestGraphPoint.first).arg(nearestGraphPoint.second));
                    } else {
                        setObjectName(QString("No Point Delete"));
                    }
                    break;
            }
            return true;
        } else if (eve->type() == QEvent::Wheel) {
                QWheelEvent *w = static_cast<QWheelEvent *>(eve);
                if (w->orientation() == Qt::Vertical)
                    transform->changeZoom(w->delta() / 120);
                cerr << w->delta();
                Point cursorScreenPoint = make_pair(w->x(), w->y());
                transform->setFocusPolicy(false, cursorScreenPoint, transform->toGraph(cursorScreenPoint));
                setObjectName(QString("Zoom: %1%").arg(transform->getZoom() * 100));
            return true;
        }
    return false;
}
