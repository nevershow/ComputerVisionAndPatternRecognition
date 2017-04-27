#ifndef H_EDGE
#define H_EDGE

#include "point.h"
using namespace std;

class Edge {
	public:
		Edge(const Point &p1, const Point &p2) : p1(p1), p2(p2) {};
		Edge(const Edge &e) : p1(e.p1), p2(e.p2) {};

		Point p1;
		Point p2;
};

inline ostream &operator << (ostream &out, Edge const &e) {
	return out << "Edge " << e.p1 << ", " << e.p2;
}

inline bool operator == (const Edge& e1, const Edge& e2) {
	return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
			(e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

#endif 

