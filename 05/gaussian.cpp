#include <iostream>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;
int ipr_gaussian(Mat_<uchar>& src, Mat_<uchar>& dst, int size, double sigma){
  if( size % 2 == 0){
    cout << "フィルタのサイズは奇数にしてください" << endl;
    return 0;
  }
  int shell;
  shell = (size - 1) / 2;
  int i;
  // 上
  for(int y = 0; y < shell; y++ ){
    for(int x = 0; x < src.cols; x++ ){
      dst(y,x) = 0;
    }
  }

  // した
  for(int y = src.rows - 1, i = 0; i < shell; i++, y--){
    for(int x = 0; x < src.cols; x++ ){
      dst(y,x) = 0;
    }
  }

  // みぎ
  for(int x = src.cols - 1, i = 0; i < shell; i++, x--){
    for(int y = 0; y < src.rows; y++){
      dst(y,x) = 0;
    }
  }

  // ひだり
  for(int x = 0; x < shell; x++){
    for(int y = 0; y < src.rows; y++){
      dst(y,x) = 0;
    }
  }
  for(int y = shell; y < src.rows - shell; y++){
    for(int x = shell; x < src.cols - shell; x++){
      float sum = 0;
      float weight;
      for(int yy = 0; yy < size; yy++){
        for(int xx = 0; xx < size; xx++){
          weight = exp((pow(yy - shell, 2) + pow(xx - shell, 2)) / (2 * pow(sigma, 2))) / (2 * pow(sigma, 2) * M_PI);
          sum += (weight * src(y - shell + yy, x - shell + xx));
        }
      }
      dst(y,x) = sum;
    }
  }
  return 0;

}
int main(int argc, char *argv[])
{
  Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
  if ( src.empty() ) {
    cerr << format("can't src (%s) open.\n", argv[1]);
    exit(1);
  }

  ipr_gaussian(src, src, 3, 1);

  imshow("kekka", src);
  waitKey(0);

  return 0;
}