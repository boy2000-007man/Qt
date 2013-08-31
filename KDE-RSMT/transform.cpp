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
    if (focusCentral) {
        graphPoint = make_pair(info[0] + 0.5 * info[2], info[1] + 0.5 * info[3]);
        screenPoint = make_pair(0.5 * widget->width(), 0.5 * widget->height());
    }
    PointType dx = (graphPoint.first - info[0]) * zoom * autoZoom - screenPoint.first;
    PointType dy = (graphPoint.second - info[1]) * zoom * autoZoom - screenPoint.second;
    return make_pair((p.first + dx) / zoom / autoZoom + info[0], (p.second + dy) / zoom / autoZoom + info[1]);
}
Point Transform::toScreen(const Point &p) {
    vector<PointType> info = graph->information();
    autoZoom = min(widget->width() / info[2], widget->height() / info[3]);
    if (focusCentral) {
        graphPoint = make_pair(info[0] + 0.5 * info[2], info[1] + 0.5 * info[3]);
        screenPoint = make_pair(0.5 * widget->width(), 0.5 * widget->height());
    }
    PointType dx = screenPoint.first - (graphPoint.first - info[0]) * zoom * autoZoom;
    PointType dy = screenPoint.second - (graphPoint.second - info[1]) * zoom * autoZoom;
    return make_pair((p.first - info[0]) * zoom * autoZoom + dx, (p.second - info[1]) * zoom * autoZoom + dy);
}
void Transform::setFocusPolicy(bool autoFocus, Point screenP, Point graphP) {
    focusCentral = autoFocus;
    screenPoint = screenP;
    graphPoint = graphP;
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
