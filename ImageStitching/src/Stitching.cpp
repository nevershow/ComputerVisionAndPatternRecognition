#include "Stitching.h"
#include "Projection.h"
#include <queue>

int getMiddleIndex(vector< vector<int> > matching_index) {
	int one_side = 0;

	for (int i = 0; i < matching_index.size(); i++) {
		if (matching_index[i].size() == 1) {
			one_side = i;
			break;
		}
	}

	int middle_index = one_side;
	int pre_middle_index = -1;
	int n = matching_index.size() / 2;
	
	while (n--) {
		for (int i = 0; i < matching_index[middle_index].size(); i++) {
			if (matching_index[middle_index][i] != pre_middle_index) {
				pre_middle_index = middle_index;
				middle_index = matching_index[middle_index][i];

				break;
			}
		}
	}

	return middle_index;
}

// 使用单应矩阵更新特征
void updateFeaturesByHomography(map<vector<float>, VlSiftKeypoint> &feature, Parameters H, float offset_x, float offset_y) {
	for (auto iter = feature.begin(); iter != feature.end(); iter++) {
		float cur_x = iter->second.x;
		float cur_y = iter->second.y;
		iter->second.x = getXAfterWarping(cur_x, cur_y, H) - offset_x;
		iter->second.y = getYAfterWarping(cur_x, cur_y, H) - offset_y;
		iter->second.ix = int(iter->second.x);
		iter->second.iy = int(iter->second.y);
	}
}

// 使用偏移更新特征
void updateFeaturesByOffset(map<vector<float>, VlSiftKeypoint> &feature, int offset_x, int offset_y) {
	for (auto iter = feature.begin(); iter != feature.end(); iter++) {
		iter->second.x -= offset_x;
		iter->second.y -= offset_y;
		iter->second.ix = int(iter->second.x);
		iter->second.iy = int(iter->second.y);
	}
}

// 
CImg<float> stitching(CImgList<float> &imgs) {
  int size = imgs.size();
	// 保存每张图片的特征和对应的坐标
	vector<map<vector<float>, VlSiftKeypoint>> features(size);

	for (int i = 0; i < size; i++) {
		cout << "开始处理图像" << i + 1 << " ...\n";

		cout << "进行柱面投影\n";
		imgs[i] = cylinderProjection(imgs[i]);
		cout << "完成柱面投影\n";

    // 转换成灰度图
		CImg<float> gray = imgs[i].get_RGBtoGray();
		cout << "进行SIFT特征提取\n";
		features[i] = getFeatures(gray);
		cout << "完成SIFT特征提取\n";

		cout << "图像 "<< i + 1 << " 处理完毕\n\n";
	}

	bool need_stitching[MAX_STITCHING_NUM][MAX_STITCHING_NUM] = { false };

	// 记录每张图片的相邻图片
	vector< vector<int> > matching_index(size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j)
				continue;

			vector<point_pair> pairs = getPointPairsFromFeature(features[i], features[j]);
			if (pairs.size() >= 20) {
				need_stitching[i][j] = true;

				matching_index[i].push_back(j);
			}

		}
	}

	cout << "开始拼接\n" << endl << endl;

	int start_index = getMiddleIndex(matching_index);

	// 记录前一张拼接的图片
	int prev_dst_index = start_index;

	queue<int> unstitched_index;
	unstitched_index.push(start_index);

	CImg<float> cur_stitched_img = imgs[start_index];

	while (!unstitched_index.empty()) {
		int src_index = unstitched_index.front();
		unstitched_index.pop();

		for (int j = matching_index[src_index].size() - 1; j >= 0; j--) {
			int dst_index = matching_index[src_index][j];

			if (!need_stitching[src_index][dst_index]) {
				continue;
			}
			else {
				need_stitching[src_index][dst_index] = false;
				need_stitching[dst_index][src_index] = false;
				unstitched_index.push(dst_index);
			}

			// 使用kd树进行特征匹配
			vector<point_pair> src_to_dst_pairs = getPointPairsFromFeature(features[src_index], features[dst_index]);
			vector<point_pair> dst_to_src_pairs = getPointPairsFromFeature(features[dst_index], features[src_index]);

			if (src_to_dst_pairs.size() > dst_to_src_pairs.size()) {
				dst_to_src_pairs.clear();
				for (int i = 0; i < src_to_dst_pairs.size(); i++) {
					point_pair temp(src_to_dst_pairs[i].b, src_to_dst_pairs[i].a);
					dst_to_src_pairs.push_back(temp);
				}
			}
			else {
				src_to_dst_pairs.clear();
				for (int i = 0; i < dst_to_src_pairs.size(); i++) {
					point_pair temp(dst_to_src_pairs[i].b, dst_to_src_pairs[i].a);
					src_to_dst_pairs.push_back(temp);
				}
			}

			// 使用RANSAC算法求单应矩阵
			Parameters forward_H = RANSAC(dst_to_src_pairs);
			Parameters backward_H = RANSAC(src_to_dst_pairs);

			// 求拼接后的图像大小
			float min_x = getMinXAfterWarping(imgs[dst_index], forward_H);
			min_x = (min_x < 0) ? min_x : 0;
			float min_y = getMinYAfterWarping(imgs[dst_index], forward_H);
			min_y = (min_y < 0) ? min_y : 0;
			float max_x = getMaxXAfterWarping(imgs[dst_index], forward_H);
			max_x = (max_x >= cur_stitched_img.width()) ? max_x : cur_stitched_img.width();
			float max_y = getMaxYAfterWarping(imgs[dst_index], forward_H);
			max_y = (max_y >= cur_stitched_img.height()) ? max_y : cur_stitched_img.height();

			int new_width = ceil(max_x - min_x);
			int new_height = ceil(max_y - min_y);

			CImg<float> a(new_width, new_height, 1, imgs[dst_index].spectrum(), 0);
			CImg<float> b(new_width, new_height, 1, imgs[dst_index].spectrum(), 0);

			warpingImageByHomography(imgs[dst_index], a, backward_H, min_x, min_y);
			movingImageByOffset(cur_stitched_img, b, min_x, min_y);

			// 根据单应矩阵更新特征
			updateFeaturesByHomography(features[dst_index], forward_H, min_x, min_y);
			// 根据偏移更新特征
			updateFeaturesByOffset(features[prev_dst_index], min_x, min_y);

			// 图像融合
			cur_stitched_img = blendTwoImages(a, b);
			prev_dst_index = dst_index;
		}
	}

	return cur_stitched_img;
}
