#pragma once
#include <string>
#include "CImg.h"           // C++ image library
using namespace std;
using namespace cimg_library;

class TestCanny {
private:
  string infile;                                // required input filename
  string outfileNMS = "Edge_Output.bmp";       // saving the binary canny edges to file
  string outfileGradient = "Gradient_Pro.bmp"; // saving the (normalised) gradient to file

  float sigma, threshold; // canny parameters

public:
  // initial input file name and canny parameters
  TestCanny(string infile, float sigma = 1.5f, float threshold = 6.0f);

  /** CannyDiscrete.
  * Version with discrete implementation of the non-maximum-suppression.
  * During the nms, the gradient and its orientation are treated discrete,
  * e.g., only four directions are considered: E, NE, N, SE.
  */
  void CannyDiscrete(CImg<float> in, float sigma, float threshold,
                     CImg<float> &outSmooth, CImg<float> &outGradient,
                     CImg<float> &outOrientation, CImg<float> &outThreshold,
                     CImg<float> &outNMS);

  // change rgb to gray
  CImg<float> RGB2Gray(CImg<float> in);

  // Test Canny with the given parameters: sigma, threshold
  int runTest();

  ~TestCanny();
};
