#include "drawwidget.h"
#include <QPainter>
using namespace std;
Drawwidget::Drawwidget(QWidget *parent) :
    QWidget(parent)
{
}
void Drawwidget::paintEvent(QPaintEvent *) {
    if (graph.points().size() == 0)
        return ;
    QPainter p(this);
    int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;
    for (int i = 0; i < graph.points().size(); i++) {
        minx = min(minx, graph.points()[i].first);
        miny = min(miny, graph.points()[i].second);
        maxx = max(maxx, graph.points()[i].first);
        maxy = max(maxy, graph.points()[i].second);
    }
    int x = maxx - minx, y = maxy - miny;
    const double k = 0.1;
    double kx = width() * (1 - k) / max(1, x);
    double ky = height() * (1 - k) / max(1, y);
    kx = ky = min(kx, ky);
    const int dx = (width() - kx * x) / 2;
    const int dy = (height() - ky * y) / 2;
    const sdk::Tuples &H = graph.HVs_.first, &V = graph.HVs_.second;

    p.setPen(QPen(Qt::black, 3));
    for (int i = 0; i < H.size(); i++)
        p.drawLine(dx + (H[i].second.first-minx) * kx, dy + (H[i].first-miny) * ky, dx + (H[i].second.second-minx) * kx, dy + (H[i].first-miny) * ky);
    for (int i = 0; i < V.size(); i++)
        p.drawLine(dx + (V[i].first-minx) * kx, dy + (V[i].second.first-miny) * ky, dx + (V[i].first-minx) * kx, dy + (V[i].second.second-miny) * ky);
    p.setPen(QPen(Qt::green, 7));
    for (int i = 0; i < graph.points().size(); i++)
        p.drawPoint(dx + (graph.points()[i].first-minx) * kx, dy + (graph.points()[i].second-miny) * ky);
}
