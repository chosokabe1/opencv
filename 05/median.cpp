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
void sort(vector<int>& x)
{
  for (vector<int>::size_type i = 0; i < x.size(); i++){
    for (vector<int>::size_type j = 0; j < x.size() - 1; j++){
      if(x[j] > x[j+1]){
        int tmp;
        tmp = x[j];
        x[j] = x[j+1];
        x[j+1] = tmp;
      }
    }
  }
}
int ipr_median_filter(Mat_<uchar>& src, Mat_<uchar>& dst){
  int shell = 1; // 外郭は1
  Mat_<uchar> tmp_image = src.clone();
  for(int y = shell; y < src.rows - shell; y++){
    for(int x = shell; x < src.cols - shell; x++){
      vector<int> a;
      for(int y2 = -1; y2 <= 1; y2++){
        for(int x2 = -1 ; x2 <= 1; x2++){
          unsigned char temp = tmp_image(y + y2, x + x2);
          a.push_back(temp);
        }
      }
      sort(a);
      int median = (a.size() + 1) / 2;
      // cout << "median = " << median << endl;
      dst(y,x) = a.at(median);

    }
  }
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
  
  return 0;

}
int main(int argc, char *argv[])
{
  Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
  if ( src.empty() ) {
    cerr << format("can't src (%s) open.\n", argv[1]);
    exit(1);
  }
  imshow("kekka", src);
  waitKey(0);

  ipr_median_filter(src, src);

  imshow("kekka", src);
  waitKey(0);

  return 0;
}