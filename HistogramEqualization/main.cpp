#include "CImg.h"
#include "Image.h"
#include <cstdlib>
#include <string>

using namespace cimg_library;
using namespace std;

int main(int argc, char const *argv[]) {

  string num[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

  // 测试灰度图和RGB图像的直方图均衡化
  for (int i = 0; i < 10; ++i) {
    string filename = "testImages/" + num[i] + ".bmp";
    CImg<float> testImg = CImg<float>(filename.c_str());
    Image rgbImg(testImg);
    Image grayImg(testImg.get_RGBtoGray());
    (testImg.get_RGBtoGray(), grayImg.histogramEqualization()).display();
    (testImg, rgbImg.HSV_HistogramEqualization()).display();
  }

  // 测试颜色转换
  for (int i = 0; i < 8; i += 2) {
    string sourceName = "testImages/1" + num[i] + ".bmp";
    string targetName = "testImages/1" + num[i + 1] + ".bmp";
    CImg<float> source(sourceName.c_str());
    CImg<float> target(targetName.c_str());
    Image image(sourceName, targetName);
    (source, target, image.colorTransfer()).display();
  }

  return 0;
}
