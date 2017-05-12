#ifndef _PROJECTION_H_
#define _PROJECTION_H_

#include "CImg.h"
#define PI 3.1415926535
using namespace cimg_library;

// 柱面投影
CImg<float> cylinderProjection(const CImg<float> &src);

#endif
