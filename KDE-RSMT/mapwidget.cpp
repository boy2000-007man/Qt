#include "mapwidget.h"
#include <QEvent>
#include <QMouseEvent>
using namespace std;
using namespace sdk;
MapWidget::MapWidget(QWidget *parent, Graph *g, Transform *t) :
    DrawWidget(parent, g, t) {
}
//void MapWidget::paintEvent(QPaintEvent *) {
//}
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
