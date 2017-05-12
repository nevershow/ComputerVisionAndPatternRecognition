#include "Projection.h"
#include "Interpolation.h"
#include <cmath>
#define ANGLE 15

// 柱面投影
CImg<float> cylinderProjection(const CImg<float> &src) {
  int width = src.width(), height = src.height();
	float r = (width / 2.0) / tan(ANGLE * PI / 180.0);
  CImg<float> res(width, height, 1, src.spectrum(), 0);
  
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float dst_x = i - width / 2;
			float dst_y = j - height / 2;

			float k = r / sqrt(r * r + dst_x * dst_x);
			float src_x = dst_x / k;
			float src_y = dst_y / k;

			if (src_x + width / 2 >= 0 && src_x + width / 2 < width
				&& src_y + height / 2 >= 0 && src_y + height / 2 < height) {
				for (int k = 0; k < res.spectrum(); k++) {
					res(i, j, k) = bilinear_interpolation(src, src_x + width / 2, src_y + height / 2, k);
				}
			}
		}
	}

	return res;
}
