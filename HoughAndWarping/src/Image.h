#include "CImg.h"
#include <bits/stdc++.h>
using namespace cimg_library;
using namespace std;

// 存储点的信息
struct dot {
  int votenum;
  double angle, r;
  dot(double angle, double r, int votenum) {
    this->angle = angle;
    this->r = r;
    this->votenum = votenum;
  }
};

class Image {
public:
  Image(const char* filename, int MAXGRADITUDE = 400, int DIFF = 100, int MAXVOTENUM = 550);  // 构造函数
  void getHoughSpace();         // 霍夫变换
  vector<dot> getDots();        // 获取霍夫空间峰值点
  void showVertexs(vector<dot> dots);   // 显示4个角点
  void warping();               // 矫正图像

private:
  int MAXGRADITUDE;         // 梯度阈值
  int DIFF;                 // 相邻霍夫空间峰值间隔
  int MAXVOTENUM;           // 投票阈值
  CImg<float> source;       // 源图像
  CImg<float> marked;       // 标记角点的图像
  CImg<float> houghspace;   // 霍夫空间
  CImg<float> result;       // 矫正后的图像
  vector<dot> vertexs;      // 4个角点
  float M[9];               // 矫正的矩阵
  vector< pair<double, double> > corner;  // 顺时针存储A4纸的角点左边

  int getX(double angle, double r, int y);  // 坐标变换
  int getY(double angle, double r, int x);  // 坐标变换
  void orderCorners();            // A4纸角点按顺时针排序
  void calcMatrix();              // 计算矫正所用的矩阵
  static bool cornerCmp(const pair<double, double> a, const pair<double, double> b); // 角点排序比较函数
};
