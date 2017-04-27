#include <vector>
#include "CImg.h"
#include "triangle.h"
using namespace std;
using namespace cimg_library;

// 坐标变换
inline Point F(const Point& p1, const Point& p2, float alpha) {
  float x = (1 - alpha) * p1.x + alpha * p2.x;
  float y = (1 - alpha) * p1.y + alpha * p2.y;
  return Point(x, y);
}

// 获取人脸特征点
void getFeaturePoints(const char* filename, std::vector<Point>& points);

// 求变换矩阵
CImg<float> getTransformMatrix(const Triangle& t1, const Triangle& t2);

// morphing 变换
CImg<float> morphing(CImg<float>&src1, CImg<float>& src2,
              std::vector<Point> points1, std::vector<Point> points2,
              float alpha);
