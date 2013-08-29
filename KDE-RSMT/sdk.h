#ifndef __SDK_H__
#define __SDK_H__
#include <vector>
#include <iostream>
namespace sdk {
typedef std::pair<int, int> Point, Pair;
std::istream& operator>>(std::istream &in, Point &point);
std::ostream& operator<<(std::ostream &out, const Point &point);
typedef std::vector<Point> Points;
typedef std::pair<int, std::pair<int, int> > Tuple;
typedef std::vector<Tuple> Tuples;
std::istream& operator>>(std::istream &in, Point &point);
std::ostream& operator<<(std::ostream &out, const Point &point);
class Edge {
    Points *points_;
    int begin_, end_;
public:
    Pair L, U;
    Edge(Points *points = NULL, int begin = 0, int end = 0);
    Tuple weight() const;
    void reverse();
    int begin() const;
    int end() const;
    Point beginp() const;
    Point endp() const;
    friend bool operator<(const Edge &e1, const Edge &e2);
    friend std::ostream& operator<<(std::ostream &out, const Edge &edge);
};  // class Edge
typedef std::vector<Edge> Edges;
bool operator<(const Edge &e1, const Edge &e2);
class Graph {
    void SMST();
    void L_RST();
    Points points_;
    std::vector<Edge> edges_;
    void calculateGraph();
public:
    Points points() const;
    void addpoint(const Point &p);
    void deletepoint(const Point &p);
    int searchpoint(const Point &p);
    std::pair<Tuples, Tuples> HVs_;
    friend std::istream& operator>>(std::istream &in, Graph &graph);
    friend std::ostream& operator<<(std::ostream &out, const Graph &graph);
};  // class Graph
}   // namespace sdk
#endif  // __SDK_H__