#include "Image.h"

const double PI = acos(-1);

// 计算两点间的距离
inline float dis(float x, float y){return sqrt(x * x + y * y);}

// 构造函数  
Image::Image(const char* filename, int MAXGRADITUDE, int DIFF, int MAXVOTENUM) {
  source = CImg<float>(filename);
  marked = source;
  this->MAXGRADITUDE = MAXGRADITUDE;
  this->DIFF = DIFF;
  this->MAXVOTENUM = MAXVOTENUM;
}

// 按角点到源图像左上角的距离排序
bool Image::cornerCmp(const pair<double, double> a, const pair<double, double> b) {
  return dis(a.first, a.second) < dis(b.first, b.second);
}

// 坐标变换
int Image::getX(double angle, double r, int y) {
  double theta = angle * PI / 180;
  double b = r / sin(theta);
  return (b - y) * tan(theta);
}

// 坐标变换
int Image::getY(double angle, double r, int x){
  double theta = angle * PI / 180;
  double m = -cos(theta) / sin(theta),
         b = r / sin(theta);
  return m * x + b;
}

// 对4个角点顺时针排序
void Image::orderCorners() {
  sort(corner.begin(), corner.end(), cornerCmp);
  double width = dis(corner[0].first - corner[1].first, corner[0].second - corner[1].second);
  double length = dis(corner[0].first - corner[2].first, corner[0].second - corner[2].second);

  // 源图像中的A4纸是横向的
  if (corner[1].first < corner[2].first && width < length) {
    swap(corner[1], corner[2]);
    swap(corner[2], corner[3]);
    corner.push_back(corner[0]);
    corner.erase(corner.begin());
  }
  // 源图像中的A4纸是竖向的
  else {
    swap(corner[2], corner[3]);
  }
}

// 计算矫正所用的矩阵
void Image::calcMatrix() {
  double x0 = corner[0].first, x1 = corner[1].first, x2 = corner[2].first, x3 = corner[3].first;
  double y0 = corner[0].second, y1 = corner[1].second, y2 = corner[2].second, y3 = corner[3].second;
  double dx3 = x0 - x1 + x2 - x3;
  double dy3 = y0 - y1 + y2 - y3;
  if (fabs(dx3) < 10e-5 && fabs(dy3) < 10e-5) {
    M[0] = x1 - x0, M[1] = y1 - y0, M[2] = 0;
    M[3] = x2 - x1, M[4] = y2 - y1, M[5] = 0;
    M[6] = x0, M[7] = y0, M[8] = 1;
  }
  else {
    double dx1 = x1 - x2, dx2 = x3 - x2,
           dy1 = y1 - y2, dy2 = y3 - y2;

    double det = dx1 * dy2 - dx2 * dy1;
    double a13 = (dx3 * dy2 - dx2 * dy3) / det;
    double a23 = (dx1 * dy3 - dx3 * dy1) / det;
    M[0] = x1 - x0 + a13 * x1, M[1] = y1 - y0 + a13 * y1, M[2] = a13;
    M[3] = x3 - x0 + a23 * x3, M[4] = y3 - y0 + a23 * y3, M[5] = a23;
    M[6] = x0, M[7] = y0, M[8] = 1;
  }
}

// 霍夫变换
void Image::getHoughSpace() {
  int width = source.width(), height = source.height();
  CImg<float> grayImg = source.get_RGBtoGray();
  CImg<float> blurImg = grayImg.get_blur_median(3).blur(3); //中值滤波 + 高斯模糊
  
  CImg_3x3(I, float);  // 算子
  houghspace = CImg<float>(360, dis(width, height), 1, 1, 0);
  height = houghspace.height();

  cimg_for3x3(blurImg, x, y, 0, 0, I, float) {
    double deltax = Inc - Ipc, deltay = Icp - Icn;
    double graditude = deltax * deltax + deltay * deltay;  // 梯度的平方，减少运算量

    // 大于梯度阈值则进行投票
    if (graditude > MAXGRADITUDE) {
      cimg_forX(houghspace, angle) {
        double theta = angle * PI / 180;
        int r = (int)(x * cos(theta) + y * sin(theta));
        if(r >= 0 && r < height)
          houghspace(angle, r) += 1;
      }
    }
  }
}

// 获取霍夫空间峰值点
vector<dot> Image::getDots() {
  int minx = 0, miny = 0,
      maxx = source.width() - 1, maxy = source.height() - 1;

  vector<dot> dots;
  cimg_forXY(houghspace, angle, r){

    int votenum = houghspace(angle, r); // 投票数
    if(votenum > MAXVOTENUM) {      // 是否为峰值
      // 图像范围
      int x0 = getX(angle, r, miny),
          x1 = getX(angle, r, maxy),
          y0 = getY(angle, r, minx),
          y1 = getY(angle, r, maxx);

      // 表示的直线不在图像内
      if ((x0 < 0 || x0 > maxx) && \
      (x1 < 0 || x1 > maxx) && \
      (y0 < 0 || y0 > maxy) && \
      (y1 < 0 || y1 > maxy)) continue;

      bool flag = false;
      for (dot &d : dots) {
        if (dis(d.angle - angle, d.r - r) < DIFF) {  // 存在相邻峰值
          flag = true;
          if (d.votenum < votenum)
            d = dot(angle, r, votenum);  // 替换为当前峰值
        }
      }
      if (!flag)
        dots.push_back(dot(angle, r, votenum));  // 加入新峰值
    }
  }

  return dots;
}

// 显示A4纸角点（投票数最高的4个点）
void Image::showVertexs(vector<dot> dots) {
  // 获取投票数最高的4个点
  sort(dots.begin(), dots.end(), [](const dot a,  const dot b) { return a.votenum > b.votenum;});
  for (dot& d : dots) {
    double theta = d.angle * PI / 180;
    double k = -cos(theta) / sin(theta),
           b = d.r / sin(theta);
    cout << "k = " << k << ", b = " << b << endl;
    vertexs.push_back(dot(k, b, -1));
    if (vertexs.size() == 4) break;
  }

  double width = marked.width(), height = marked.height();
  float blue[] = {0, 0, 255};

  // 标出角点
  for(int i = 0; i < vertexs.size(); ++i) {
    for(int j = i + 1; j < vertexs.size(); ++j) {
      double k0 = vertexs[i].angle,
             b0 = vertexs[i].r,
             k1 = vertexs[j].angle,
             b1 = vertexs[j].r;
      double x = (b1 - b0) / (k0 - k1),
             y = (k0 * b1 - k1 * b0) / (k0 - k1);
      cout << "x = " << x << ", y = " << y << endl;
      if( x >= 0 && y >= 0 && x < width && y < height) {
        marked.draw_circle(x, y, 50, blue);
        corner.push_back(make_pair(x, y));
      }
    }
  }
  // marked.display();
}

// 图像矫正
void Image::warping() {
  orderCorners();
  result = source;
  result.resize(1050, 1485);  // 标准A4纸比例

  double width = result.width(), height = result.height();

  calcMatrix();
  cimg_forXY(result, x, y) {
    double _x = x / width, _y = y / height;
    double denominator = M[2] * _x + M[5] * _y + M[8];
    double tx = (M[0] * _x + M[3] * _y + M[6]) / denominator;
    double ty = (M[1] * _x + M[4] * _y + M[7]) / denominator;
    cimg_forC(result, c) {
      result(x, y, c) = source((int)tx, (int)ty, c);
    }
  }

  (source, marked, result).display();
}
