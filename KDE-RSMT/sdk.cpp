#include "sdk.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
using namespace std;
using namespace sdk;
double sdk::distance(const Point &p1, const Point &p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}
bool sdk::equal(const Point &p1, const Point &p2) {
    return abs(sdk::distance(p1, p2) < ZERO);
}
bool sdk::equal(const PointType &p1, const PointType &p2) {
    return abs(p1 - p2) < ZERO;
}
Point sdk::convert(const Point &p, double kx, double ky, double dx, double dy) {
    return make_pair(p.first * kx + dx, p.second * ky + dy);
}
Point sdk::reconvert(const Point &p, double kx, double ky, double dx, double dy) {
    return make_pair((p.first - dx) / kx, (p.second - dy) / ky);
}
istream& sdk::operator>>(istream &in, Point &point) {
    return in >> point.first >> point.second;
}
ostream& sdk::operator<<(ostream &out, const Point &point) {
    return out << "[" << point.first << ", " << point.second << "]";
}
Edge::Edge(Points *points, int begin, int end) : points_(points), begin_(begin), end_(end), L(make_pair(-1, 0)), U(make_pair(-1, 0)){
}
void Edge::reverse() {
    int tmp = begin_;
    begin_ = end_;
    end_ = tmp;
}
int Edge::begin() const{
    return begin_;
}
int Edge::end() const{
    return end_;
}
Point Edge::beginp() const {
    return (*points_)[begin_];
}
Point Edge::endp() const {
    return (*points_)[end_];
}
ostream& sdk::operator<<(ostream &out, const Edge &edge) {
    return out << edge.begin() << "->" << edge.end() << endl
        << edge.L.first << "," << edge.L.second << endl
        << edge.U.first << "," << edge.U.second << endl;
}
static PointType dist(const Point &p1, const Point &p2) {
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}
Tuple Edge::weight() const {
    const Point &p1 = (*points_)[begin_], &p2 = (*points_)[end_];
    return make_pair(dist(p1, p2), make_pair(-abs(p1.second - p2.second), -max(p1.first, p2.first)));
}
bool sdk::operator<(const Edge &e1, const Edge &e2) {
    return e1.weight() < e2.weight();
}
static int findroot(vector<int> &root, int order) {
    if (root[order] == order)
        return order;
    return root[order] = findroot(root, root[order]);
}
void Graph::SMST() {
    sort(points_.begin(), points_.end());
    /*Points points;
    for (int i = 0; i < points_.size(); i++) {
        points.push_back(points_[i]);
        while (i < points_.size() && equal(points_[i], points_[i + 1]))
            i++;
    }
    points_ = points;*/
    Edges tmp;
    for (int i = 0; i < points_.size(); i++)
        for (int j = i + 1; j < points_.size(); j++)
            tmp.push_back(Edge(&points_, i, j));
    sort(tmp.begin(), tmp.end());
    vector<int> root(points_.size());
    for (int i = 0; i < root.size(); i++)
        root[i] = i;
    edges_.clear();
    for (int i = 0; i < tmp.size() && edges_.size() < points_.size() - 1; i++) {
        int r1 = findroot(root, tmp[i].begin());
        int r2 = findroot(root, tmp[i].end());
        if (r1 != r2) {
            edges_.push_back(tmp[i]);
            root[max(r1, r2)] = min(r1, r2);
        }
    }
}
static void pretreatment(vector<vector<int> > &edges, Edges &edges_, int origion, int root) {
    for (int i = 0; i < edges[root].size(); i++) {
        Edge &e = edges_[edges[root][i]];
        if (e.begin() != origion) {
            if (e.begin() != root)
                e.reverse();
            pretreatment(edges, edges_, e.begin(), e.end());
        }
    }
    for (int i = 0; i < edges[root].size(); i++) {
        Edge &e = edges_[edges[root][i]];
        if (e.begin() == origion) {
            int tmp = edges[root].back();
            edges[root][edges[root].size() - 1] = edges[root][i];
            edges[root][i] = tmp;
            break;
        }
    }
}
static Point make_sortpair(PointType a, PointType b) {
    return make_pair(min(a, b), max(a, b));
}
static pair<Tuple, Tuple> hv(const Point &p1, const Point &p2, int LU) {
    PointPair h(make_sortpair(p1.first, p2.first));
    PointPair v(make_sortpair(p1.second, p2.second));
    Tuple h_ = make_pair(LU % 2 == 0 ? min(p1.second, p2.second) : max(p1.second, p2.second), h);
    Tuple v_ = make_pair(((p1.second < p2.second) ^ (LU % 2 == 0)) ? p1.first : p2.first, v);
    return make_pair(h_, v_);
}
static PointType overlapmax(const vector<int> &edges, const Edges edges_, int LU) {
    Tuples H, V;
    for (int i = 0; i < edges.size(); i++) {
        const Edge &e = edges_[edges[i]];
        const pair<Tuple, Tuple> hv_ = hv(e.beginp(), e.endp(), LU >> edges.size() - i - 1);
        H.push_back(hv_.first);
        V.push_back(hv_.second);
    }
    sort(H.begin(), H.end());
    sort(V.begin(), V.end());
    /*if (edges.back() == 0) {
        cerr << "H:" << endl;
        for (int i = 0; i < H.size(); i++)
            cerr << H[i].first << " " << H[i].second << endl;
        cerr << "V:" << endl;
        for (int i = 0; i < V.size(); i++)
            cerr << V[i].first << " " << V[i].second << endl;
    }*/
    PointType overlapsum = 0;
    for (int i = 0; i < H.size(); i++)
        overlapsum += H[i].second.second - H[i].second.first;
    for (int i = 0; i < V.size(); i++)
        overlapsum += V[i].second.second - V[i].second.first;
    for (int i = 1; i < H.size(); i++)
        if (equal(H[i - 1].first, H[i].first) && H[i - 1].second.second > H[i].second.first)
            H[i - 1].second.second = H[i].second.first;
    for (int i = 1; i < V.size(); i++)
        if (equal(V[i - 1].first, V[i].first) && V[i - 1].second.second > V[i].second.first)
            V[i - 1].second.second = V[i].second.first;
    for (int i = 0; i < H.size(); i++)
        overlapsum -= H[i].second.second - H[i].second.first;
    for (int i = 0; i < V.size(); i++)
        overlapsum -= V[i].second.second - V[i].second.first;
    return overlapsum;
}
static void LU(const vector<vector<int> > &edges, Edges &edges_, Edge &e) {
    Pair &L = e.L;
    Pair &U = e.U;
    if (L.first > 0 && U.first > 0)
        return ;
    if (edges[e.end()].size() == 1) {
        L = U = make_pair(dist(e.beginp(), e.endp()), 0);
        return ;
    }
    for (int i = 0; i < (1 << edges[e.end()].size()); i++) {
        PointType tmp = dist(e.beginp(), e.endp()) - overlapmax(edges[e.end()], edges_, i);
        for (int j = 0; j < edges[e.end()].size() - 1; j++) {
            LU(edges, edges_, edges_[edges[e.end()][j]]);
            if ((i >> edges[e.end()].size() - j - 1) % 2 == 0)
                tmp += edges_[edges[e.end()][j]].L.first;
            else
                tmp += edges_[edges[e.end()][j]].U.first;
        }
    //cerr << tmp << " " << i << endl;
        if (i % 2 == 0 && (L.first > tmp || L.first < 0))
            L = make_pair(tmp, i);
        if (i % 2 == 1 && (U.first > tmp || U.first < 0))
            U = make_pair(tmp, i);
    }
}
istream& sdk::operator>>(istream &in, Graph &graph) {
    graph.points_.clear();
    int n;
    in >> n;
    Point tmp;
    for (int i = 0; i < n; i++) {
        in >> tmp;
        graph.points_.push_back(tmp);
    }
    graph.calculateGraph();
    return in;
}
static pair<Tuples, Tuples> HV(const vector<vector<int> > &edges, const Edges &edges_, const Edge &e, int LU) {
    Tuples H, V;
    for (int i = 0; i < edges[e.end()].size() - 1; i++) {
        const Edge &n_e = edges_[edges[e.end()][i]];
        const pair<Tuple, Tuple> hv_ = hv(n_e.beginp(), n_e.endp(), LU >> edges[e.end()].size() - i - 1);
        H.push_back(hv_.first);
        V.push_back(hv_.second);
        pair<Tuples, Tuples> HVs = HV(edges, edges_, n_e, (LU >> edges[e.end()].size() - i - 1) % 2 == 0 ? n_e.L.second : n_e.U.second);
        for (int i = 0; i < HVs.first.size(); i++)
            H.push_back(HVs.first[i]);
        for (int i = 0; i < HVs.second.size(); i++)
            V.push_back(HVs.second[i]);
    }
    return make_pair(H, V);
}
void Graph::L_RST() {
    vector<vector<int> > edges(points_.size());
    for (int i = 0; i < edges_.size(); i++) {
        edges[edges_[i].begin()].push_back(i);
        edges[edges_[i].end()].push_back(i);
    }
    int root = 0;
    while (edges[root].size() != 1)
        root++;
    pretreatment(edges, edges_, -1, root);
    Edge &e = edges_[edges[root][0]];
    LU(edges, edges_, e);
    HVs_ = HV(edges, edges_, e, e.L.first < e.U.first ? e.L.second : e.U.second);
    const pair<Tuple, Tuple> hv_ = hv(e.beginp(), e.endp(), e.L.first > e.U.first);
    HVs_.first.push_back(hv_.first);
    HVs_.second.push_back(hv_.second);
    sort(HVs_.first.begin(), HVs_.first.end());
    sort(HVs_.second.begin(), HVs_.second.end());
}
ostream& sdk::operator<<(ostream &out, const Graph &graph) {
    out << "P:";
    for (int i = 0; i < graph.points_.size(); i++)
        out << graph.points_[i] << endl;
    out << "E:";
    for (int i = 0; i < graph.edges_.size(); i++)
        out << graph.edges_[i] << endl;
    out << "H:";
    for (int i = 0; i < graph.HVs_.first.size(); i++) {
        const Tuple &H = graph.HVs_.first[i];
        out << H.first << ","
            << H.second.first << "-" << H.second.second << endl;
    }
    out << "V:";
    for (int i = 0; i < graph.HVs_.second.size(); i++) {
        const Tuple &V = graph.HVs_.second[i];
        out << V.first << ","
            << V.second.first << "-" << V.second.second << endl;
    }
    return out;
}
void Graph::calculateGraph() {
    if (points_.size() < 2) {
        HVs_.first.clear();
        HVs_.second.clear();
        return ;
    }
    SMST();
    L_RST();
}
Points Graph::points() const {
    return points_;
}
int Graph::searchPoint(const Point &p) const {
    int l = 0, r = points_.size() - 1;
    while (l < r) {
        if (equal(points_[(l + r) / 2], p))
            l = r = (l + r) / 2;
        else if (points_[(l + r) / 2] < p)
            l = (l + r) / 2 + 1;
        else if (points_[(l + r) / 2] > p)
            r = (l + r) / 2;
    }
    return equal(points_[l], p) ? l : -1;
}
Point Graph::searchNearestPoint(const Point &p) const {
    int nearestPointNumber = 0;
    for (int i = 1; i < points_.size(); i++)
        if (sdk::distance(p, points_[i]) < sdk::distance(p, points_[nearestPointNumber]))
            nearestPointNumber = i;
    return points_[nearestPointNumber];
}
bool Graph::addPoint(const Point &p) {
    points_.push_back(p);
    calculateGraph();
    return true;
}
bool Graph::deletePoint(const Point &p) {
    int loc = searchPoint(p);
    if (loc == -1)
        return false;
    points_[loc] = points_.back();
    points_.pop_back();
    calculateGraph();
    return true;
}
vector<PointType> Graph::information() const {
    PointType minx = points_[0].first, miny = points_[0].second, maxx = points_[0].first, maxy = points_[0].second;
    for (int i = 1; i < points_.size(); i++) {
        minx = min(minx, points_[i].first);
        miny = min(miny, points_[i].second);
        maxx = max(maxx, points_[i].first);
        maxy = max(maxy, points_[i].second);
    }
    vector<PointType> info(4);
    info[0] = minx;
    info[1] = miny;
    info[2] = maxx - minx;
    info[3] = maxy - miny;
    return info;
}
void Graph::setPoints(const Points &ps) {
    points_ = ps;
    calculateGraph();
}
void Graph::changePoint(const Point &op, const Point &np) {
    points_[searchPoint(op)] = np;
    calculateGraph();
}
