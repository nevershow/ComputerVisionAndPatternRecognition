#include "Image.h"
#include "mnist.h"

// 按行切割
CImgList<unsigned char> cutRow(CImg<unsigned char>& img) {
  CImgList<unsigned char> rows;
  int row_start = 0, row_end = -1;
  while (row_end < img.height()) {
    row_start = row_end + 1;
    for (int i = row_start; i < img.height(); ++i) {
      int ct = 0;
      for (int j = 0; j < img.width(); ++j)
        if (img(j, i) == 255)
          ++ct;

      if (ct > 8) {
        row_start = i;
        break;
      }
    }
    
    row_end = row_start;
    for (int i = row_start + 1; i < img.height(); ++i) {
      int ct = 0;
      for (int j = 0; j < img.width(); ++j)
        if (img(j, i) == 255)
          ++ct;
    
      if (ct < 5) {
        row_end = i;
        break;
      }
    }
    
    if (row_end - row_start > 30) {
      CImg<unsigned char> row = CImg<unsigned char>(img.width(), row_end - row_start, 1, 1, 255);
      for (int i = 0; i < row.height(); ++i)
        for (int j = 0; j < row.width(); ++j)
          row(j, i) = img(j, row_start + i);

      rows.push_back(row);
    }
  }

  return rows;
}

// 按列切割
CImgList<unsigned char> cutCol(CImg<unsigned char>& img) {
  CImgList<unsigned char> res;
  int col_start = 0, col_end = -1;

  while (col_end < img.width()) {
    col_start = col_end + 1;
    for (int i = col_start; i < img.width(); ++i) {
      int ct = 0;
      for (int j = 0; j < img.height(); ++j)
        if (img(i, j) == 255)
          ++ct;

      if (ct > 5) {
        col_start = i;
        break;
      }
    }
     
    col_end = col_start;
    for (int i = col_start + 1; i < img.width(); ++i) {
      int ct = 0;
      for (int j = 0; j < img.height(); ++j)
        if (img(i, j) == 255)
          ++ct;
    
      if (ct < 5) {
        col_end = i;
        break;
      }
    }
    
    if (col_end - col_start > 5) {
      CImg<unsigned char> col = CImg<unsigned char>(img.height(), col_end - col_start, 1, 1, 255);
      for (int i = 0; i < col.width(); ++i)
          for (int j = 0; j < col.height(); ++j)
            col(i, j) = img(col_start + i, j);

        res.push_back(col);
    }
  }
  return res;
}

int main(int argc, char const *argv[]) {
  // A4 矫正

  // int MAXGRADITUDE = atoi(argv[2]), 
  //     DIFF = atoi(argv[3]),
  //     MAXVOTENUM = atoi(argv[4]);

  // Image img(argv[1], MAXGRADITUDE, DIFF, MAXVOTENUM);
  // img.getHoughSpace();
  // img.showVertexs(img.getDots());
  // img.warping();
  
  int threshold[] = {160, 130, 140, 155, 160};
  for (int i = 1; i <= 5; ++i) {
    string name = "images/input/";
    name += to_string(i) + ".jpg";
    CImg<unsigned char> img(name.c_str());
    img = img.get_RGBtoGray();
    cimg_forXY(img, x, y) {
      img(x, y) = img(x, y) < threshold[i - 1] ? 255 : 0;
    }
    
    CImgList<unsigned char> rows = cutRow(img);
    for (int j = 1; j <= rows.size(); ++j) {
      string name = "images/output/";
      name += to_string(i) + "/" + to_string(j) + ".jpg";
      rows[j - 1].save(name.c_str());
    }
    rows.display();
    
    CImgList<unsigned char> res;
    
    for (auto row : rows) {
      CImgList<unsigned char> cols = cutCol(row);
      for (auto col : cols) {
        res.push_back(col);
      }
    }
  }

  return 0;
}
