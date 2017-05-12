#include "Feature.h"

map<vector<float>, VlSiftKeypoint>  getFeatures(const CImg<float> &input) {
	CImg<float> src(input);

	float resize_factor;
	if (input.width() < input.height()) {
		resize_factor = RESIZE_SIZE / input.width();
	}
	else {
		resize_factor = RESIZE_SIZE / input.height();
	}

	if (resize_factor >= 1) {
		resize_factor = 1;
	}
	else {
		src.resize(src.width() * resize_factor, src.height() * resize_factor, src.depth(), src.spectrum(), 3);
	}

	vl_sift_pix *imageData = new vl_sift_pix[src.width()*src.height()];

	for (int i = 0; i < src.width(); i++) {
		for (int j = 0; j < src.height(); j++) {
			imageData[j*src.width() + i] = src(i, j, 0);
		}
	}

	// 创建一个新的sift滤波器
	int noctaves = 4, nlevels = 2, o_min = 0;
	VlSiftFilt *siftFilt = NULL;
	siftFilt = vl_sift_new(src.width(), src.height(), noctaves, nlevels, o_min);

	map<vector<float>, VlSiftKeypoint> features;

	// Compute the first octave of the DOG scale space.
	if (vl_sift_process_first_octave(siftFilt, imageData) != VL_ERR_EOF) {
		while (true) {
			// 计算每组中的关键点
			vl_sift_detect(siftFilt);

			// 遍历并绘制每个点
      VlSiftKeypoint *pKeyPoint = siftFilt->keys;  //检测到的关键点的数目
			for (int i = 0; i < siftFilt->nkeys; i++) {
				VlSiftKeypoint tempKeyPoint = *pKeyPoint;

				// 计算并遍历每个点的方向
				double angles[4];
				int angleCount = vl_sift_calc_keypoint_orientations(siftFilt, angles, &tempKeyPoint);

				for (int j = 0; j < angleCount; j++) {
					double tempAngle = angles[j];
					vl_sift_pix descriptors[128];

					// 计算每个方向的描述
					vl_sift_calc_keypoint_descriptor(siftFilt, descriptors, &tempKeyPoint, tempAngle);

					vector<float> des;
					int k = 0;
					while (k < 128) {
						des.push_back(descriptors[k]);
						k++;
					}

					tempKeyPoint.x /= resize_factor;
					tempKeyPoint.y /= resize_factor;
					tempKeyPoint.ix = tempKeyPoint.x;
					tempKeyPoint.iy = tempKeyPoint.y;

					features.insert(pair<vector<float>, VlSiftKeypoint>(des, tempKeyPoint));
				}

				pKeyPoint++;
			}
      // 下一阶
			if (vl_sift_process_next_octave(siftFilt) == VL_ERR_EOF) {
				break;
			}
		}
	}

	vl_sift_delete(siftFilt);

	delete[] imageData;
	imageData = NULL;

	return features;
}
