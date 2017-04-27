#ifndef H_DELAUNAY
#define H_DELAUNAY

#include "point.h"
#include "triangle.h"
#include <vector>
using namespace std;

class Delaunay {
	public:
		void triangulate(vector<Point> &points);
		const vector<Triangle>& getTriangles() const { return _triangles; };

	private:
		vector<Triangle> _triangles;
};

#endif
