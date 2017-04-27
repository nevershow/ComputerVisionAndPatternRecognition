#include "triangle.h"

Triangle::Triangle(const Point &_p1, const Point &_p2, const Point &_p3) :	
					p1(_p1), p2(_p2), p3(_p3), e1(_p1, _p2), e2(_p2, _p3), e3(_p3, _p1) {}

// 判断点p是否是三角形的顶点
bool Triangle::containsVertex(const Point &p) {
	return p1 == p || p2 == p || p3 == p; 
}

// 判断点p是否在三角形的外接圆内
bool Triangle::circumCircleContains(const Point &p) {
	float ab = (p1.x * p1.x) + (p1.y * p1.y);
	float cd = (p2.x * p2.x) + (p2.y * p2.y);
	float ef = (p3.x * p3.x) + (p3.y * p3.y);

	// 圆心(x, y), 半径sqrt(radius2)
	float X = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2;
	float Y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2;
	float radius2 = ((p1.x - X) * (p1.x - X)) + ((p1.y - Y) * (p1.y - Y));

	// 点到圆心的距离小于半径即在园内
	float dis = ((p.x - X) * (p.x - X)) + ((p.y - Y) * (p.y - Y));
	return dis <= radius2;
}

// 判断点P是否在三角形内
// 参考链接（http://blog.csdn.net/dracularking/archive/2008/03/25/2217180.aspx）
// 计算三个向量两两叉乘，同号表示在三角形内
bool Triangle::isPointInTriangle(Point& p) {
	Point pA = p - p1;
	Point pB = p - p2;
	Point pC = p - p3;
	float a = pA * pB, b = pB * pC, c = pC * pA;
	return (a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0);
}
