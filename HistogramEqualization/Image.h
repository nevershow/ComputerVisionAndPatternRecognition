#include "CImg.h"
#include <string>
using namespace std;
using namespace cimg_library;

class Image {
public:
  Image(string source, string target = "");
  Image(CImg<float> gray);  // 灰度图进行初始化

  ~Image();
  CImg<float>& histogramEqualization();       // 直方图均衡化
  CImg<float>& HSV_HistogramEqualization();   // HSV空间的直方图均衡化
  CImg<float>& colorTransfer();               // 颜色转移

private:
  CImg<float> source;  // 源图像
  CImg<float> target;  // 目标图像
  CImg<float> result;  // 结果图像

  float* histogram;    // 直方图
  float* cdf;          // 累积分布函数

  int minpixel;        // 像素最小值
  int maxpixel;        // 像素最大值

  // 计算直方图和概率密度函数
  void _getHistogram(const unsigned int channel, const unsigned int levels = 256);

  // 均衡化
  void _equalize(const unsigned int channel, const unsigned int levels = 256);
};
