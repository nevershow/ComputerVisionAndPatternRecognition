#include "delaunay.h"
#include <algorithm>
using namespace std;

// 三角剖分
// Bowyer-Watson算法(推荐)
// 算法的基本步骤是：
// 1、构造一个超级三角形，包含所有散点，放入三角形链表。
// 2、将点集中的散点依次插入，在三角形链表中找出外接圆包含插入点的三角形（称为该点的影响三角形），删除影响三角形的公共边，将插入点同影响三角形的全部顶点连接起来，完成一个点在Delaunay三角形链表中的插入。
// 3、根据优化准则对局部新形成的三角形优化。将形成的三角形放入Delaunay三角形链表。
// 4、循环执行上述第2步，直到所有散点插入完毕。
void Delaunay::triangulate(vector<Point> &points) {
	// 求超级三角形
	float minX = points[0].x;
	float minY = points[0].y;
	float maxX = minX;
	float maxY = minY;

	for (Point p : points) {
		if (p.x < minX) minX = p.x;
  	if (p.y < minY) minY = p.y;
  	if (p.x > maxX) maxX = p.x;
  	if (p.y > maxY) maxY = p.y;
	}
	
	float dx = maxX - minX, dy = maxY - minY;
	float deltaMax = max(dx, dy);
	float midx = (minX + maxX) / 2, midy = (minY + maxY) / 2;

	Point p1(midx - 20 * deltaMax, midy - deltaMax);
	Point p2(midx, midy + 20 * deltaMax);
	Point p3(midx + 20 * deltaMax, midy - deltaMax);	

	// 将超级三角形加入三角形列表
	_triangles.push_back(Triangle(p1, p2, p3));

	for(Point p : points) {
		vector<Triangle> badTriangles;
		vector<Edge> polygon;

		for(Triangle t : _triangles) {
			if(t.circumCircleContains(p)) {
				badTriangles.push_back(t);
				polygon.push_back(t.e1);
				polygon.push_back(t.e2);
				polygon.push_back(t.e3);
			}
		}

		_triangles.erase(remove_if(begin(_triangles), end(_triangles), [badTriangles](Triangle &t) {
			for(Triangle bt : badTriangles) {	
				if (bt == t) return true;		
			}
			return false;
		}), end(_triangles));

		vector<Edge> badEdges;
		int size = polygon.size();
		for(int i = 0; i < size; ++i) {
			for(int j = i + 1; j < size; ++j) {
				if (polygon[i] == polygon[j]) {
					badEdges.push_back(polygon[i]);	
					badEdges.push_back(polygon[j]);	
				}
			}
		}

		polygon.erase(remove_if(begin(polygon), end(polygon), [badEdges](Edge &e){
			for(Edge be : badEdges) {
				if (be == e) return true;
			}
			return false;
		}), end(polygon));

		for(Edge e : polygon)
			_triangles.push_back(Triangle(e.p1, e.p2, p));
	}
	

	_triangles.erase(remove_if(begin(_triangles), end(_triangles), [p1, p2, p3](Triangle &t){
		return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
	}), end(_triangles));
}
