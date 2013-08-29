#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "sdk.h"
class Drawwidget : public QWidget
{
    Q_OBJECT
public:
    explicit Drawwidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    sdk::Graph graph;
signals:
    
public slots:
    
};

#endif // DRAWWIDGET_H
