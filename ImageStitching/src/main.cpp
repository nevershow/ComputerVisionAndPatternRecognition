#include "CImg.h"
#include "Stitching.h"

using namespace std;
using namespace cimg_library;

int main(int argc, char **argv) {
	char filename[20];
	CImgList<float> imgs;
	for (int i = 1; i <= 4; ++i) {
		sprintf(filename, "dataset1/%d.jpg", i);
		imgs.push_back(CImg<float>(filename));
	}
	CImg<float> result1 = stitching(imgs);
	result1.display();
	result1.save("result/result1.jpg");
  
  imgs.clear();

  for (int i = 1; i <= 18; ++i) {
    sprintf(filename, "dataset2/%d.jpg", i);
    imgs.push_back(CImg<float>(filename));
  }
  CImg<float> result2 = stitching(imgs);
  result2.display();
  result2.save("result/result2.jpg");

	return 0;
}
