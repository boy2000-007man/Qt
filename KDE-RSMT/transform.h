#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "sdk.h"
#include <QWidget>
namespace sdk {
class Transform
{
    Graph *graph;
    QWidget *widget;
    bool focusCentral;
    Point screenPoint, graphPoint;
    double zoom, zoomRate, autoZoom;
public:
    Transform(QWidget *w, Graph *g);
    Point toScreen(const Point &p);
    Point toGraph(const Point &p);
    void setFocusPolicy(bool autoFocus = true);
    void changeZoom(double k);
    void setZoom(double z);
    void setZoomRate(double zoomR);
    void setScreenPoint(const Point &sp);
    void setGraphPoint(const Point &gp);
    double getZoom();
};
}

#endif // TRANSFORM_H
