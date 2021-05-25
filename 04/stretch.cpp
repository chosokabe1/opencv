#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp> //normalize
#include "save_histogram.cpp"

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_stretch(Mat_<uchar>& src, Mat_<uchar>&dst, int dst_min, int dst_max);
void normalize_stretch(Mat_<uchar>& src, Mat_<uchar>&dst, int dst_min, int dst_max);
void ipr_minMaxLoc(Mat_<uchar>& src, double& src_min, double& src_max);
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

  
  int dst_min = 0;
  int dst_max = 255;
  //自作関数を使う場合
  ipr_stretch(src,dst,dst_min,dst_max);

  //existing
  // double src_min,src_max;
  // minMaxLoc(src,&src_min,&src_max);

  //OpenCVの関数normalizeを使う場合
  // normalize(src, dst, dst_max, dst_min, NORM_MINMAX);

  //Matクラスのメソットconverttoを使う場合
  //src.convertTo(dst, -1, 1, - src_min); // 引数  出力、-1の場合同じ型で出力、かける、たす
  //dst.convertTo(dst, -1, (dst_max - dst_min) / (src_max - src_min), dst_min);

  ipr_calc_histogram(dst, hist);
  ipr_save_histogram(hist, "lenna_stretch1.dat");


  return 0;
}

void ipr_stretch(Mat_<uchar>& src, Mat_<uchar>&dst, int dst_min, int dst_max){
  double src_min,src_max;
  ipr_minMaxLoc(src, src_min, src_max);
  cout << format("%f %f\n", src_min,src_max);
  for(int y = 0; y < src.rows; y++ ){
    for(int x = 0; x < src.cols;x++ ){
      dst(y,x) = (dst_max - dst_min) * (src(y,x) - src_min ) / (src_max - src_min) + dst_min; 
    }
  }
}
void ipr_minMaxLoc(Mat_<uchar>& src, double& src_min, double& src_max){
  Mat_<float> hist(NBIN, 1, 0.0);
  ipr_calc_histogram(src,hist);
  src_min = hist(0,0);
  src_max = hist(0,0);
  for( int i = 0; i < NBIN; i++ ){
    if( hist(i,0) != 0.0){
      src_min = i;
      break;
    }
  }
  for( int i = NBIN -1; i > 0; i--){
    if( hist(i,0) > 0){
      src_max = i;
      break;
    }
  }
}


