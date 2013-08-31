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
    void setPointSize(double s);
    void setLineSize(double s);
private:
    double pointSize, lineSize;
signals:
public slots:
    void targetTo(double x, double y);
};

#endif // DRAWWIDGET_H
