#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;
std::stringstream buf;
int main(int argc, char *argv[]){
  Mat_<Vec3b> src = imread(argv[1], IMREAD_COLOR);
  if ( src.empty() ) {
    cerr << format("can't src (%s) open.\n", argv[1]);
    exit(1);
  }
  imshow("kekka", src);
  waitKey(0);

  for(int frame = 0; frame < 100; frame++){
    buf.str("");
    buf.clear(stringstream::goodbit);
    buf << cv::format("result%03d.png", frame);
    imwrite(buf.str(), src);
  }

  return 0;

}