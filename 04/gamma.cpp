#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp> //normalize,LUT
#include <opencv2/core/utility.hpp>
#include "save_histogram.cpp"

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;
void ipr_gamma_trans(Mat_<uchar>& src, Mat_<uchar>& dst, double gamma);
void gamma_trans(Mat_<uchar>& src, Mat_<uchar>& dst, double gamma);
void ipr_gamma_lut_trans(Mat_<uchar>& src, Mat_<uchar>& dst, double gamma);

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

  double src_min,src_max;
  minMaxLoc(src,&src_min,&src_max);
  normalize(src, src, dst_max, dst_min, NORM_MINMAX);

  double gamma= 1.2;
  // 二重ループ内で変換関数を計算する
  ipr_gamma_trans(src, dst, gamma);

  // 関数LUTを使う方法
  gamma_trans(src, dst, gamma);

  // ルックアップテーブルを用いる方法
  ipr_gamma_lut_trans(src, dst, gamma);

  imshow("src", src);
  waitKey(0);

  imshow("dst", dst);
  waitKey(0);

  return 0;
}

void ipr_gamma_trans(Mat_<uchar>& src, Mat_<uchar>& dst, double gamma){
  TickMeter meter;
  meter.start();
  for(int t = 0; t < 1000; t++){
    for(int y = 0; y < src.rows; y++){
      for(int x = 0; x < src.cols; x++){
         dst(y,x) = 255 * pow(src(y,x) / 255.0, 1/gamma);
     }
    }
  }  
  meter.stop();
  cout << "ipr_gamma: " << meter.getTimeMilli() << "ms" << endl;
}
void gamma_trans(Mat_<uchar>& src, Mat_<uchar>& dst, double gamma){
  Mat_<float> look(NBIN, 1, 0.0);
  Mat_<uchar> lut(NBIN, 1, 0.0);

  TickMeter meter;
  meter.start();
  for(int t = 0; t < 1000; t++){
    for(int i = 0; i < NBIN; i++){
      look(i,0) = 255 * pow(i / 255.0, 1/gamma);
      lut(i,0) = look(i,0);
    }
    LUT(src,lut,dst);
  }
  meter.stop();
  cout << "lut: " << meter.getTimeMilli() << "ms" << endl;
}
void ipr_gamma_lut_trans(Mat_<uchar>& src, Mat_<uchar>& dst, double gamma){
  Mat_<float> look(NBIN, 1, 0.0);
  Mat_<uchar> lut(NBIN, 1, 0.0);
  TickMeter meter;
  meter.start();
  for(int t = 0; t < 1000; t++){

    for(int i = 0; i < NBIN; i++){
      look(i,0) = 255 * pow(i / 255.0, 1/gamma);
      lut(i,0) = look(i,0);
    }
    for(int y = 0; y < src.rows; y++){
      for(int x = 0; x < src.cols; x++){
        dst(y,x) = lut(src(y,x), 0);
      }
    }
  }
  meter.stop();
  cout << "ipr_lut: " << meter.getTimeMilli() << "ms" << endl;
}