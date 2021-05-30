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

  Mat_<int> aha(100, 1, 0);
  float tmp = 0;
  for(int i = 0; i < 100; i++){
    tmp += NBIN / 100.0;
    aha(i,0) = tmp;
    cout << "aha(" << i << ",0) = " << aha(i,0) << endl; 
  }
  Mat gray_src;
  Mat_<uchar> gray_dst(src.rows, src.cols);

  cvtColor(src,gray_src,COLOR_BGR2GRAY);
  imshow("kekka", gray_src);
  waitKey(0);

  threshold(gray_src,gray_dst,64,NBIN-1,THRESH_BINARY_INV);
  for(int y = 0; y < gray_dst.rows; y++){
    for(int x = 0; x < gray_dst.cols; x++){
      if(x < 490 || y > 295){
        gray_dst(y,x) = 0;
      }
    }
  }
   imshow("kekka", gray_dst);
  waitKey(0);
  morphologyEx(gray_dst,gray_dst,MORPH_OPEN,Mat(),Point(-1,-1),1);
  morphologyEx(gray_dst,gray_dst,MORPH_CLOSE,Mat(),Point(-1,-1),4);
  morphologyEx(gray_dst,gray_dst,MORPH_OPEN,Mat(),Point(-1,-1),2);

  imshow("kekka", gray_dst);
  waitKey(0);
  for(int frame = 0; frame < 100; frame++ ) {
    for(int y = 0; y < gray_dst.rows; y++){
      for(int x = 0; x < gray_dst.cols; x++){
        if(gray_dst(y,x) == 255){
          src(y,x)[0] = aha(frame,0);
          src(y,x)[1] = aha(frame,0);
          src(y,x)[2] = aha(frame,0);
        }
      }
    }

    buf.str("");
    buf.clear(stringstream::goodbit);
    buf << cv::format("result%03d.png", frame);
    imwrite(buf.str(), src);
  }

  return 0;

}