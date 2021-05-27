#include <iostream>
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
  for(int y = shell; y < src.rows - shell; y++){
    for(int x = shell; x < src.cols - shell; x++){
      vector<int> x;
      for(int yy = -1; yy <= 1; yy++){
        for(int xx = -1 ; xx <= 1; xx++){
          int temp = src(y + yy, x + xx);
          x.push_back(temp);
          sort(x);
          for (vector<int>::size_type i = 0; i < x.size(); i++){
            cout << "x[" << i << "] = " << x[i] << endl;
          }
          cout << "x.size() = " << x.size() << endl;
        }
      }

      

      dst(y,x) = sum;
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