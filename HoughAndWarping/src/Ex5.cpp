#include "Image.h"

int main(int argc, char const *argv[]) {
  int MAXGRADITUDE = atoi(argv[2]), 
      DIFF = atoi(argv[3]),
      MAXVOTENUM = atoi(argv[4]);

  Image img(argv[1], MAXGRADITUDE, DIFF, MAXVOTENUM);
  img.getHoughSpace();
  img.showVertexs(img.getDots());
  img.warping();
  
  return 0;
}
