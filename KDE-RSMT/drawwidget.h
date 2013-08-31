#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "sdk.h"
#include "transform.h"
class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0, sdk::Graph *g = 0, sdk::Transform *t = 0);
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *, QEvent *);
    sdk::Graph *graph;
    sdk::Transform *transform;
private:
    sdk::Point pointSelected;
    bool selectPoint;
signals:

public slots:
    
};

#endif // DRAWWIDGET_H
