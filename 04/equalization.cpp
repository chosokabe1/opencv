#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp> //normalize, LUT
#include "save_histogram.cpp"

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_equalization(Mat_<uchar>& src, Mat_<uchar>& dst);
int main(int argc, char *argv[])
{
  Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
  if ( src.empty() ) {
    cerr << format("can't src (%s) open.\n", argv[1]);
    exit(1);
  }

  Mat_<float> hist(NBIN, 1, 0.0);
  ipr_calc_histogram(src, hist);

  Mat_<uchar> dst(src.rows, src.cols, 1);

  // ipr_equalization(src,dst);
  equalizeHist(src,dst);
  imshow("src", src);
  waitKey(0);

  imshow("dst", dst);
  waitKey(0);
  ipr_calc_histogram(src, hist);
  ipr_save_histogram(hist, "lenna.dat");
  ipr_calc_histogram(dst, hist);
  ipr_save_histogram(hist, "equalization.dat");


  return 0;
}

void ipr_equalization(Mat_<uchar>& src , Mat_<uchar>& dst){
  Mat_<float> hist(NBIN, 1, 0.0);
  ipr_calc_histogram(src, hist);
  float sum = 0;
  for ( int i = 0; i < NBIN; i++ ){
    sum += hist(i,0);
  }

  Mat_<float> look(NBIN, 1, 0.0);
  for( int i = 0; i < NBIN; i++){
    int sum_local = 0;
    for( int j = 0; j < i; j++){
      sum_local += hist(j,0);
    }
    look(i,0) = 1.0 * NBIN * sum_local / sum;
  }
  for(int y  = 0; y < src.rows; y++){
    for(int x = 0; x < src.cols; x++){
      dst(y,x) = look(src(y,x),0);
    }
  }
}



