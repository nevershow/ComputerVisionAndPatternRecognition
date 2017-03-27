#include "Image.h"
#include <cmath>

// 使用文件名进行初始化
Image::Image(string sourceName, string targetName) {
  if (!sourceName.empty())
    source = CImg<float>(sourceName.c_str());
  if (!targetName.empty())
    target = CImg<float>(targetName.c_str());

  histogram = 0;
  cdf = 0;
}

// 使用灰度图进行初始化
Image::Image(CImg<float> gray) {
  source = gray;
  histogram = 0;
  cdf = 0;
}

// 释放资源
Image::~Image() {
  if (histogram != 0) {
    delete []histogram;
    histogram = 0;
  }
  if (cdf != 0) {
    delete []cdf;
    cdf = 0;
  }
}

// 计算直方图和概率密度函数
void Image::_getHistogram(const unsigned int channel, const unsigned int levels) {
  if (histogram == 0) {
    histogram = new float[levels];
  }
  for (int i = 0; i < levels; ++i) {
    histogram[i] = 0;
  }

  minpixel = levels, maxpixel = 0;
  cimg_forXY(source, x, y) {
    int pixel = source(x, y, channel);
    histogram[pixel]++;
    minpixel = min(minpixel, pixel);
    maxpixel = max(maxpixel, pixel);
  }
  int area = source.width() * source.height();
  for (int i = 0; i < levels; ++i) {
    histogram[i] /= area;
  }
}

// 计算累积分布函数并进行均衡化
void Image::_equalize(const unsigned int channel, const unsigned int levels) {
  if (cdf == 0)
    cdf = new float[levels];
  for (int i = 0; i < levels; ++i) {
    cdf[i] = 0;
  }
  cdf[0] = histogram[0];
  for (int i = 1; i < levels; ++i) {
    cdf[i] = cdf[i - 1] + histogram[i];
  }

  // 对每一通道分别进行均衡化
  cimg_forXY(source, x, y) {
    result(x, y, channel) = cdf[(int)source(x, y, channel)] * (maxpixel - minpixel) + minpixel;
  }
}

// 直方图均衡化
CImg<float>& Image::histogramEqualization() {
  result = CImg<float>(source);

  // 对每一通道分别处理
  for (int channel = 0; channel < source.spectrum(); ++channel) {
    this->_getHistogram(channel);
    this->_equalize(channel);
  }

  return result;
}

// 把RGB空间转为HSV空间，再进行直方图均衡化
CImg<float>& Image::HSV_HistogramEqualization() {
  source = source.RGBtoHSV();

  // V通道取值范围为[0,1], 先转化为[0,255]
  cimg_forXY(source, x, y) {
    source(x, y, 2) *= 255;
  }

  // 对V通道进行直方图均衡化
  result = CImg<float>(source);
  this->_getHistogram(2);
  this->_equalize(2);

  // 将V通道转化为原来的[0,1]
  cimg_forXY(result, x, y) {
    result(x, y, 2) /= 255;
  }
  return result.HSVtoRGB();
}

// 颜色转移
CImg<float>& Image::colorTransfer() {
  // RGB空间转化为Lab空间
  source = source.RGBtoLab();
  target = target.RGBtoLab();
  result = CImg<float>(source);

  // 高斯分布变换
  double sourceMean = source.mean(),
         sourceDeviation = sqrt(source.variance()),
         targetMean = target.mean(),
         targetDeviation = sqrt(target.variance());

  cimg_forXY(source, x, y) {
    for (int i = 0; i < 3; i++) {
      result(x, y, i) = (source(x, y, i) - sourceMean) * targetDeviation / sourceDeviation + targetMean;
    }
  }

  return result.LabtoRGB();
}
