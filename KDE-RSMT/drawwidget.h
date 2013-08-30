#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "sdk.h"
class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *, QEvent *);
    sdk::Graph graph;
    void changeZoom(int k);
private:
    double zoom;
    bool selectPoint;
    sdk::Point selectedPoint;
signals:

public slots:
    
};

#endif // DRAWWIDGET_H
