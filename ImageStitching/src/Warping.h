#ifndef _WARPING_H_
#define _WARPING_H_

#include <iostream>
#include "Match.h"
#include "Interpolation.h"

using namespace std;

float getMaxXAfterWarping(const CImg<float> &src, Parameters H);
float getMinXAfterWarping(const CImg<float> &src, Parameters H);
float getMaxYAfterWarping(const CImg<float> &src, Parameters H);
float getMinYAfterWarping(const CImg<float> &src, Parameters H);

int getWidthAfterWarping(const CImg<float> &src, Parameters H);
int getHeightAfterWarping(const CImg<float> &src, Parameters H);

void warpingImageByHomography(const CImg<float> &src, CImg<float> &dst, Parameters H, float offset_x, float offset_y);
void movingImageByOffset(const CImg<float> &src, CImg<float> &dst, int offset_x, int offset_y);


#endif
