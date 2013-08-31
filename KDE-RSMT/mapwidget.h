#ifndef MAPWIDGET_H
#define MAPWIDGET_H
#include "drawwidget.h"

class MapWidget : public DrawWidget
{
    Q_OBJECT
public:
    MapWidget(QWidget *parent = 0, sdk::Graph *g = 0, sdk::Transform *t = 0);
    //void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *, QEvent *);
signals:
    void clickPoint(double, double);
};

#endif // MAPWIDGET_H
