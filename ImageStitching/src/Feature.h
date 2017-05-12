#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <map>
#include <vector>
#include "CImg.h"

#define RESIZE_SIZE 500.0

using namespace cimg_library;
using namespace std;

extern "C" {
	#include "vl/generic.h"
	#include "vl/sift.h"
}

map<vector<float>, VlSiftKeypoint> getFeatures(const CImg<float> &src);

#endif
