#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;
const int NBIN = 256;   // ヒストグラムのビン数

int main(int argc, char *argv[])
{
  Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
  Mat_<uchar> dst(src.rows, src.cols);
  bitwise_not(src,src);
  threshold(src,dst,3,NBIN-1,THRESH_OTSU);
  // imshow("1", dst);
  // waitKey(0);
  
  // erode(dst,dst,Mat(),Point(-1,-1),1);
  // imshow("1", dst);
  // waitKey(0);

  // dilate(dst,dst,Mat(),Point(-1,-1),1);
  // imshow("1", dst);
  // waitKey(0);

  morphologyEx(dst,dst,MORPH_OPEN,Mat(),Point(-1,-1),1);
  imshow("1", dst);
  waitKey(0);

  // morphologyEx(dst,dst,MORPH_CLOSE,Mat(),Point(-1,-1),1);
  // imshow("1", dst);
  // waitKey(0);

  Mat_<int> labels(dst.rows, dst.cols, 0);
  connectedComponents(dst,labels,8);
  imshow("1", dst);
  waitKey(0);

  //cout << labels << endl;
  return 0;
}
