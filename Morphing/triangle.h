#ifndef H_TRIANGLE
#define H_TRIANGLE

#include "edge.h"
#include "point.h"
using namespace std;

class Triangle {
	public:
		Triangle(const Point &_p1, const Point &_p2, const Point &_p3);
	
		bool containsVertex(const Point &p);  // 判断点p是否是三角形的顶点
		bool circumCircleContains(const Point &p);  // 判断点p是否在三角形的外接圆内
		bool isPointInTriangle(Point& p);  // 判断点p是否在三角形内(同向法)

		Point p1, p2, p3;
		Edge e1, e2, e3;
};

inline ostream &operator << (ostream &out, const Triangle & t) {
	return out << "Triangle:" << "\n\t" << t.p1 << "\n\t" << t.p2 << "\n\t" << t.p3 << "\n\t" << t.e1 << "\n\t" << t.e2 << "\n\t" << t.e3 << endl;
}

inline bool operator == (const Triangle &t1, const Triangle &t2) {
	return	(t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3) &&
			(t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3) && 
			(t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3);
}

#endif
