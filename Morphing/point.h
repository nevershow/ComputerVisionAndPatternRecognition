#ifndef H_POINT
#define H_POINT
#include <iostream>
using namespace std;

class Point {
	public:
		Point(float _x = 0, float _y = 0, int _index = -1) : x(_x), y(_y), index(_index) {}

		Point(const Point &p) {
			x = p.x, y = p.y, index = p.index;
		}

    // 叉乘
		inline float operator*(Point& p) {
      return x * p.y - p.x * y;
    }

    inline Point operator-(Point& p) {
    	return Point(x - p.x, y - p.y);
    }

		float x;
		float y;
		int index;
};

inline ostream &operator << (ostream &out, Point const &point)  {
	return out << "Point x: " << point.x << " y: " << point.y;
}

inline bool operator == (const Point& p1, const Point& p2) {
	return (p1.x == p2.x) && (p1.y == p2.y);
}
	
#endif
