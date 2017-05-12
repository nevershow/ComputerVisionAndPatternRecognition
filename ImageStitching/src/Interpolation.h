#ifndef _INTERPOLATION_H_
#define _INTERPOLATION_H_

#include "CImg.h"
#include <cmath>

using namespace std;
using namespace cimg_library;

// 双线性插值
template <class T>
T bilinear_interpolation(const CImg<T>& image, float x, float y, int channel) {
  int x_pos = floor(x);
  float x_u = x - x_pos;
  int xb = (x_pos < image.width() - 1) ? x_pos + 1 : x_pos;

  int y_pos = floor(y);
  float y_v = y - y_pos;
  int yb = (y_pos < image.height() - 1) ? y_pos + 1 : y_pos;

  float P1 = image(x_pos, y_pos, channel) * (1 - x_u) + image(xb, y_pos, channel) * x_u;
  float P2 = image(x_pos, yb, channel) * (1 - x_u) + image(xb, yb, channel) * x_u;

  return P1 * (1 - y_v) + P2 * y_v;
}

#endif
