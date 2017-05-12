#ifndef _STITCHING_H_
#define _STITCHING_H_

#include "Blend.h"
#include "Feature.h"
#include "Warping.h"

#define MAX_STITCHING_NUM 20

CImg<float> stitching(CImgList<float> &imgs);

#endif
