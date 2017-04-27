#include "morphing.h"
#include <string>
using namespace std;

int main() {
  const char* filename1 = "Image/a.bmp";
  const char* filename2 = "Image/b.bmp";
  float red[] = {255, 0, 0};
  std::vector<Point> points1, points2;
  CImg<float> src1(filename1), src2(filename2);
  CImgList<float> gif;
  gif.push_back(src1); // 添加原图像

  // 获取特征点
  getFeaturePoints(filename1, points1);
  getFeaturePoints(filename2, points2);

  string resultname[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
  for (int i = 1; i < 11; ++i) {
    float alpha = 1.f / 11 * i;
    CImg<float> result = morphing(src1, src2, points1, points2, alpha);
    string filename = "Image/";
    filename += resultname[i - 1];
    filename += ".bmp";
    result.save(filename.c_str());
    gif.push_back(result); // 添加动画帧
  }

  gif.push_back(src2); // 添加目标图像
  gif.save_gif_external("Image/result.gif", 250); // 保存动画帧

  return 0;
}
