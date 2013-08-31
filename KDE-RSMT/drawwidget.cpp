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
    graph(g), pointSize(7.0), lineSize(3.0),
    transform(t) {
}
void DrawWidget::targetTo(double x, double y) {
    transform->setFocusPolicy(false);
    transform->setGraphPoint(make_pair(x, y));
    transform->setScreenPoint(make_pair(0.5 * width(), 0.5 * height()));
    setObjectName(QString("Target To Point: [%1, %2]").arg(x).arg(y));
}
void DrawWidget::setPointSize(double s) {
    pointSize = s;
}
void DrawWidget::setLineSize(double s) {
    lineSize = s;
}
void DrawWidget::paintEvent(QPaintEvent *) {
    if (graph->points().size() == 0)
        return ;

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, lineSize));
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

    painter.setPen(QPen(Qt::green, pointSize));
    for (int i = 0; i < graph->points().size(); i++) {
        const Point p = transform->toScreen(graph->points()[i]);
        painter.drawPoint(p.first, p.second);
    }
}
bool DrawWidget::eventFilter(QObject *obj, QEvent *eve) {
    static int mouseStatus = 0;
    static sdk::Point pointSelected;
    if (obj == this)
        if (graph->points().size() == 0)
            return false;
        else if (eve->type() == QEvent::MouseMove) {
            QMouseEvent *m = static_cast<QMouseEvent *>(eve);
            Point cursorScreenPoint = make_pair(m->x(), m->y());
            Point cursorGraphPoint = transform->toGraph(cursorScreenPoint);

            if (mouseStatus == 1) {

                graph->deletePoint(pointSelected);
                graph->addPoint(pointSelected = cursorGraphPoint);
                setObjectName(QString("Move Point: [%1, %2]").arg(cursorGraphPoint.first).arg(cursorGraphPoint.second));
            } else if (mouseStatus == 2){

                transform->setScreenPoint(cursorScreenPoint);
                transform->setFocusPolicy(false);
                cerr << "cursorPoint" << m->x() << "," << m->y() << endl;
                setObjectName(QString("Move Screen: [%1, %2]").arg(cursorScreenPoint.first).arg(cursorScreenPoint.second));
            }
            return true;
        } else if (eve->type() == QEvent::MouseButtonPress) {
            QMouseEvent *m = static_cast<QMouseEvent *>(eve);

            Point cursorScreenPoint = make_pair(m->x(), m->y());
            Point cursorGraphPoint = transform->toGraph(cursorScreenPoint);
            Point nearestGraphPoint = graph->searchNearestPoint(cursorGraphPoint);
            Point nearestScreenPoint = transform->toScreen(nearestGraphPoint);
            //PointType shortestGraphDistance = sdk::distance(cursorGraphPoint, nearestGraphPoint);
            PointType shortestScreenDistance = sdk::distance(cursorScreenPoint, nearestScreenPoint);

            mouseStatus = 0;
            switch (m->button()) {
                case Qt::LeftButton:
                    if (shortestScreenDistance > pointSize) {
                        graph->addPoint(cursorGraphPoint);
                        setObjectName(QString("Add Point: [%1, %2]").arg(cursorGraphPoint.first).arg(cursorGraphPoint.second));
                    } else {
                        mouseStatus = 1;
                        pointSelected = nearestGraphPoint;
                        setObjectName(QString("Select Point: [%1, %2]").arg(pointSelected.first).arg(pointSelected.second));
                    }
                    break;
                case Qt::RightButton:
                    if (shortestScreenDistance < pointSize) {
                        graph->deletePoint(nearestGraphPoint);
                        setObjectName(QString("Delete Point: [%1, %2]").arg(nearestGraphPoint.first).arg(nearestGraphPoint.second));
                    } else {
                        mouseStatus = 2;
                        transform->setScreenPoint(cursorScreenPoint);
                        transform->setGraphPoint(cursorGraphPoint);
                        cerr << "cursorGraphPoint" << cursorGraphPoint.first << "," << cursorGraphPoint.second << endl;
                        setObjectName(QString("Ready To Move"));
                    }
                    break;
                default:
                    break;
            }
            return true;
        } else if (eve->type() == QEvent::Wheel) {
                QWheelEvent *w = static_cast<QWheelEvent *>(eve);
                if (w->orientation() == Qt::Vertical)
                    transform->changeZoom(w->delta() / 120);
                cerr << w->delta();
                Point cursorScreenPoint = make_pair(w->x(), w->y());
                transform->setGraphPoint(transform->toGraph(cursorScreenPoint));
                transform->setScreenPoint(cursorScreenPoint);
                transform->setFocusPolicy(false);
                setObjectName(QString("Zoom: %1%").arg(transform->getZoom() * 100));
            return true;
        }
    return false;
}
