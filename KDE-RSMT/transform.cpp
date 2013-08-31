#include "transform.h"
#include <cmath>
using namespace std;
using namespace sdk;
Transform::Transform(QWidget *w, Graph *g) : graph(g), widget(w), focusCentral(true), zoom(1.0), zoomRate(1.1)
{
}
Point Transform::toGraph(const Point &p) {
    vector<PointType> info = graph->information();
    autoZoom = min(widget->width() / info[2], widget->height() / info[3]);
    autoZoom = min(autoZoom, (double)INT_MAX);
    Point gPoint = graphPoint;
    Point sPoint = screenPoint;
    if (focusCentral) {
        gPoint = make_pair(info[0] + 0.5 * info[2], info[1] + 0.5 * info[3]);
        sPoint = make_pair(0.5 * widget->width(), 0.5 * widget->height());
    }
    PointType dx = (gPoint.first - info[0]) * zoom * autoZoom - sPoint.first;
    PointType dy = (gPoint.second - info[1]) * zoom * autoZoom - sPoint.second;
    return make_pair((p.first + dx) / zoom / autoZoom + info[0], (p.second + dy) / zoom / autoZoom + info[1]);
}
Point Transform::toScreen(const Point &p) {
    vector<PointType> info = graph->information();
    autoZoom = min(widget->width() / info[2], widget->height() / info[3]);
    autoZoom = min(autoZoom, (double)INT_MAX);
    Point gPoint = graphPoint;
    Point sPoint = screenPoint;
    if (focusCentral) {
        gPoint = make_pair(info[0] + 0.5 * info[2], info[1] + 0.5 * info[3]);
        sPoint = make_pair(0.5 * widget->width(), 0.5 * widget->height());
    }
    PointType dx = sPoint.first - (gPoint.first - info[0]) * zoom * autoZoom;
    PointType dy = sPoint.second - (gPoint.second - info[1]) * zoom * autoZoom;
    return make_pair((p.first - info[0]) * zoom * autoZoom + dx, (p.second - info[1]) * zoom * autoZoom + dy);
}
void Transform::setFocusPolicy(bool autoFocus) {
    focusCentral = autoFocus;
}
void Transform::changeZoom(double k) {
    zoom *= pow(zoomRate, k);
}
void Transform::setZoom(double z) {
    zoom = z;
}
void Transform::setZoomRate(double zoomR) {
    zoomRate = zoomR;
}
void Transform::setScreenPoint(const Point &sp) {
    screenPoint = sp;
}
void Transform::setGraphPoint(const Point &gp) {
    graphPoint = gp;
}
double Transform::getZoom() {
    return zoom;
}
