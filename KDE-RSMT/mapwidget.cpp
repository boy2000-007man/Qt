#include "mapwidget.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
using namespace std;
using namespace sdk;
MapWidget::MapWidget(QWidget *parent, Graph *g, Transform *t) :
    DrawWidget(parent, g, t) {
}
void MapWidget::setDrawWidget(DrawWidget *dw) {
    drawWidget = dw;
}

void MapWidget::paintEvent(QPaintEvent *p) {
    if (graph->points().size() == 0)
        return ;
    DrawWidget::paintEvent(p);

    Point graphLeftButtom = drawWidget->transform->toGraph(make_pair(0, 0));
    Point graphRigtTop = drawWidget->transform->toGraph(make_pair(drawWidget->width(), drawWidget->height()));
    Point sLB = transform->toScreen(graphLeftButtom);
    Point sRT = transform->toScreen(graphRigtTop);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(255, 0, 0, 125)));
    painter.drawRect(sLB.first, sLB.second, sRT.first - sLB.first, sRT.second - sLB.second);
    painter.setPen(QPen(QColor(0, 0, 255), 1.5 * pointSize));
    painter.drawPoint((sLB.first + sRT.first) * 0.5, (sLB.second + sRT.second) * 0.5);
}
bool MapWidget::eventFilter(QObject *obj, QEvent *eve) {
    if (obj == this)
        if (graph->points().size() == 0)
            return false;
        else if (eve->type() == QEvent::MouseButtonPress) {
            cerr << "pressbutton" << endl;
            QMouseEvent *m = static_cast<QMouseEvent *>(eve);
            if (m->button() != Qt::LeftButton)
                return false;
            Point cursorScreenPoint = make_pair(m->x(), m->y());
            Point cursorGraphPoint = transform->toGraph(cursorScreenPoint);
            emit clickPoint(cursorGraphPoint.first, cursorGraphPoint.second);
            cerr << "cursorG" << cursorGraphPoint.first << "," << cursorGraphPoint.second << endl;
            return true;
        } else if (eve->type() == QEvent::MouseMove) {
            cerr << "moveG";
            QMouseEvent *m = static_cast<QMouseEvent *>(eve);
            Point cursorScreenPoint = make_pair(m->x(), m->y());
            Point cursorGraphPoint = transform->toGraph(cursorScreenPoint);
            emit clickPoint(cursorGraphPoint.first, cursorGraphPoint.second);
            return true;
        }
    return false;
}
