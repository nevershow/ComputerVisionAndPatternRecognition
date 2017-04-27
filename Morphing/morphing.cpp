#include <vector>
#include "delaunay.h"
#include "morphing.h"
#include "dlib/image_io.h"
#include "dlib/image_processing.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/image_processing/render_face_detections.h"
using namespace std;
using namespace dlib;


// 获取人脸特征点
void getFeaturePoints(const char* filename, std::vector<Point>& points) {
  CImg<float> src(filename);

  frontal_face_detector detector = get_frontal_face_detector(); // 人脸探测器
  array2d<rgb_pixel> img;
  load_image(img, filename);
  std::vector<rectangle> faces = detector(img);  // 返回探测到的所有人脸
  shape_predictor sp;
  deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
  full_object_detection shape = sp(img, faces[0]);
  
  // 显示人脸特征点
  float red[] = {255, 0, 0};
  for (int i = 0; i < shape.num_parts(); ++i) {
    point p = shape.part(i);
    src.draw_circle(p.x(), p.y(), 2, red);
    points.push_back(Point(p.x(), p.y()));
  }
  src.display();
}

// 获取变换矩阵
// !!! CImg 下标是先y后x !!!
// 坑爹, 以后要注意
CImg<float> getTransformMatrix(const Triangle& t1, const Triangle& t2) {
  CImg<float> m1(3, 3);
  m1(0, 0) = t1.p1.x;
  m1(1, 0) = t1.p2.x;
  m1(2, 0) = t1.p3.x;
  m1(0, 1) = t1.p1.y;
  m1(1, 1) = t1.p2.y;
  m1(2, 1) = t1.p3.y;
  m1(0, 2) = m1(1, 2) = m1(2, 2) = 1;

  CImg<float> m2(3, 3);
  m2(0, 0) = t2.p1.x;
  m2(1, 0) = t2.p2.x;
  m2(2, 0) = t2.p3.x;
  m2(0, 1) = t2.p1.y;
  m2(1, 1) = t2.p2.y;
  m2(2, 1) = t2.p3.y;
  m2(0, 2) = m2(1, 2) = m2(2, 2) = 1;

  return m2 / m1;
}

// morphing 变换
CImg<float> morphing(CImg<float>&src1, CImg<float>& src2,
              std::vector<Point> points1, std::vector<Point> points2,
              float alpha) {
  int width = src1.width(), height = src1.height();
  int red[] = {255, 0, 0};

  // 生成中间过渡图特征点
  std::vector<Point> midpoints;
  int size = points1.size();
  for (int j = 0; j < size; ++j) {
    midpoints.push_back(F(points1[j], points2[j], alpha));
    midpoints[j].index = j;  // 记录顶点的下标
  }

  // 三角剖分
  Delaunay triangulation;
  triangulation.triangulate(midpoints);
  std::vector<Triangle> triangles = triangulation.getTriangles();

  // 存储中间图到原图像和目标图像每个三角形对应的变换矩阵
  std::vector< CImg<float> > mat1, mat2;
  size = triangles.size();
  for (int j = 0; j < size; ++j) {
    Triangle mid = triangles[j]; // 中间过渡三角形mid
    // 对应原图像和目标图像中的三角形a, b
    Triangle a = Triangle(points1[mid.p1.index], points1[mid.p2.index], points1[mid.p3.index]);
    Triangle b = Triangle(points2[mid.p1.index], points2[mid.p2.index], points2[mid.p3.index]);
    
    // mid到a和mid到b的变换矩阵
    mat1.push_back(getTransformMatrix(mid, a));
    mat2.push_back(getTransformMatrix(mid, b));
  }

  // 显示剖分的三角形
  CImg<float> target1 = src1;
  for (Triangle t : triangles) {
    target1.draw_triangle(t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y, red, 1.f, ~0U);
  }
  target1.display();
  
  // 对原图像进行三角变换
  cimg_forXY(target1, x, y) {
    bool flag = false;
    Point p(x, y);
    int size = triangles.size();
    for (int i = 0; i < size; ++i) {
      if (triangles[i].isPointInTriangle(p)) {
        // 计算中间图像对应于原图像的坐标
        // CImg 坐标先y后x
        float tx = x * mat1[i](0, 0) + y * mat1[i](1, 0) + mat1[i](2, 0);
        float ty = x * mat1[i](0, 1) + y * mat1[i](1, 1) + mat1[i](2, 1);
        int _x = (int)(tx), _y = (int)(ty);

        if (_x >= 0 && _x < width && _y >= 0 && _y < height) {
          cimg_forC(src1, c)
            target1(x, y, c) = src1.linear_atXY(_x, _y, c);
        }
        flag = true; 
        break;
      }
    }
    if (!flag) {
      cimg_forC(src1, c)
        target1(x, y, c) = src1.linear_atXY(x, y, c);
    }
  }

  // 显示剖分的三角形
  CImg<float> target2 = src2;
  for (Triangle t : triangles) {
    target2.draw_triangle(t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y, red, 1.f, ~0U);
  }
  target2.display();

  //对目标图像进行三角变换
  cimg_forXY(target2, x, y) {
    bool flag = false;
    Point p(x, y);
    int size = triangles.size();
    for (int i = 0; i < size; ++i) {
      if (triangles[i].isPointInTriangle(p)) {
        // 计算中间图像对应于目标图像的坐标
        // CImg 坐标先y后x
        float tx = x * mat2[i](0, 0) + y * mat2[i](1, 0) + mat2[i](2, 0);
        float ty = x * mat2[i](0, 1) + y * mat2[i](1, 1) + mat2[i](2, 1);
        int _x = (int)(tx), _y = (int)(ty);
        
        if (_x >= 0 && _x < width && _y >= 0 && _y < height) {
          cimg_forC(src2, c)
            target2(x, y, c) = src2.linear_atXY(_x, _y, c);
        }

        flag = true;
        break;
      }
    }
    if (!flag) {
      cimg_forC(src2, c)
        target2(x, y, c) = src2.linear_atXY(x, y, c);
    }
  }
  
  // 像素加权
  CImg<float> result = src1;
  cimg_forXYC(result, x, y, c)
    result(x, y, c) = alpha * target2(x, y, c) + (1 - alpha) * target1(x, y, c);


  (target1, target2, result).display();
  // result.display();

  return result;
}
