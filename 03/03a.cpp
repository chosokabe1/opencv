#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/core.hpp> //hconcat
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "save_histogram.cpp"

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_threshold(Mat_<uchar> &src,Mat_<uchar> &dst,unsigned char threshold,unsigned char max_value);
unsigned char ipr_threshold_otsu(Mat_<uchar> &src ,Mat_<uchar> &dst,unsigned char max_value);
void ipr_erode(Mat_<uchar>& src,Mat_<uchar>& dst,int iteration);
void ipr_dilate(Mat_<uchar>& src,Mat_<uchar>& dst,int iteration);
Point minPoint(vector<Point> contours);
Point maxPoint(vector<Point> contours);


void ipr_morphologyEx(
  Mat_<uchar>& src, Mat_<uchar>& dst,
  int op,
  int iteration
)
{
  if(op == MORPH_OPEN){
    ipr_dilate(src,dst,iteration);
  }else{
    ipr_erode(src,dst,iteration);
  }
}
void labeling(Mat_<int>& labels, int x, int y, int label){  
  if(x < 0 || x >= labels.cols) return;
  if(y < 0 || y >= labels.rows) return;
  if(labels(y,x) != -1) return;

  labels(y,x) = label;

  //8近傍
  labeling(labels, x + 1, y + 1, label);
  labeling(labels, x    , y + 1, label);
  labeling(labels, x - 1, y + 1, label);
  labeling(labels, x + 1, y    , label);
  labeling(labels, x - 1, y    , label);
  labeling(labels, x + 1, y - 1, label);
  labeling(labels, x    , y    , label);
  labeling(labels, x - 1, y + 1, label);
}
int ipr_labeling(
  Mat_<uchar>& src,
  Mat_<int>& labels
)
{
  //ラベルの初期化
  //０出ないところを-1にする
  for(int y = 0; y < src.rows; y++){
    for(int x = 0; x < src.cols; x++){
      if(src(y,x) != 0){
        labels(y,x) = -1;
      }
    }
  }
  //左上から-1を探す
  int label = 1;
  for(int y = 0; y < src.rows; y++){
    for(int x = 0; x < src.cols; x++){
      if(labels(y,x) == -1){
        labeling(labels, x, y, label++);
      }
    }
  }
  return label;
}
void disturber(Mat_<int>& labels, Mat_<float>& hist_rate, int label)
{
  for(int y = 0; y < labels.rows; y++){
    for(int x = 0; x < labels.cols; x++){
      if(hist_rate(labels(y,x), 0) < 0.01 || hist_rate(labels(y,x), 0) > 0.06)
      {
        labels(y,x) = 0;
      }
    }
  }
}
void ipr_hist_rate(Mat_<float>& hist, Mat_<float>& hist_rate){
  float hist_sum = 0;
  for(int i = 0;i < hist.rows; i++){
    hist_sum += hist(i,0);
  } 
  for(int i = 0; i < hist.rows; i++){
    hist_rate(i,0) = hist(i,0) / hist_sum;
  }
  ipr_save_int_image_histogram(hist_rate, "hist_rate3.dat");
}
void after_labeling_src(Mat_<uchar>& src, Mat_<int>& labels){
  for(int y = 0; y < src.rows; y++){
    for(int x = 0; x < src.cols; x++){
      if(labels(y,x) == 0){
        src(y,x) = 0;
      }
    }
  }
}
void coloring_src(Mat_<uchar>& src, Mat_<Vec3b>& dst){
  for(int y = 0; y < src.rows; y++){
    for(int x = 0; x < src.cols; x++){
      if( src(y,x) == 255){
        dst(y,x)[2] = 100;
      }
    }
  }
}
void rectan(Mat_<uchar>& src, Mat_<Vec3b>& dst){
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
  for(int i = 0; i < contours.size(); i++){
    Point minP = minPoint(contours.at(i) );
    Point maxP = maxPoint(contours.at(i) );
    Rect rect(minP, maxP );
    rectangle(dst, rect, Scalar(0, 255, 0), 2 ,8);
  }
}

int main(int argc, char *argv[])
{

  Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
  Mat_<uchar> dst(src.rows, src.cols);
  Mat_<Vec3b> dst1(src.rows, src.cols);
  Mat_<Vec3b> dst2(src.rows, src.cols);
  Mat_<Vec3b> dst3(src.rows, src.cols);
  Mat_<Vec3b> dst4(src.rows, src.cols);
  Mat_<Vec3b> dst5(src.rows, src.cols);

  cvtColor(src,dst1,COLOR_GRAY2RGB);
  dst = src.clone();
  //にちか
  const unsigned char max_value = 255;
  bitwise_not(dst,dst);
  ipr_threshold_otsu(dst,dst,max_value);
  cvtColor(dst,dst2,COLOR_GRAY2RGB);
  //opening.closing
  ipr_morphologyEx(dst,dst,MORPH_CLOSE,1);
  ipr_morphologyEx(dst,dst,MORPH_OPEN,1);
  cvtColor(dst,dst3,COLOR_GRAY2RGB);
  //labeling
  Mat_<int> labels(dst.rows, dst.cols, 0);
  int label = ipr_labeling(dst,labels);
  Mat_<float> hist(label, 1, 0.0);
  Mat_<float> hist_rate(label, 1, 0.0);
  ipr_calc_int_image_histogram(labels,hist);
  ipr_hist_rate(hist,hist_rate);
  disturber(labels,hist_rate,label);
  Mat_<float> disturber_hist(label, 1, 0.0);
  ipr_calc_int_image_histogram(labels,disturber_hist);
  after_labeling_src(dst,labels);
  cvtColor(dst,dst4,COLOR_GRAY2RGB);
  //結果の表示
  Mat_<Vec3b> color_src;
  cvtColor(src,color_src,COLOR_GRAY2RGB);
  coloring_src(dst,color_src);
  rectan(dst,color_src);
  dst5 = color_src.clone();

  Mat matArray [] = {dst1,dst2,dst3,dst4,dst5,};
  Mat_<Vec3b> dst6(src.rows * 5, src.cols);
  vconcat(matArray,5,dst6);
  imshow("5", dst6);
  waitKey(0);
  imwrite("kekka.png", dst6);

  return 0;
}
//最小座標を求める
Point minPoint(vector<Point> contours){
  double minx = contours.at(0).x;
  double miny = contours.at(0).y;
  for(int i=1;i<contours.size(); i++){
    if(minx > contours.at(i).x){
      minx = contours.at(i).x;
    }
    if(miny > contours.at(i).y){
      miny = contours.at(i).y;
    }
  }
  return Point(minx, miny);
}
//最大座標を求める
Point maxPoint(vector<Point> contours){
  double maxx = contours.at(0).x;
  double maxy = contours.at(0).y;
  for(int i=1;i<contours.size(); i++){
    if(maxx < contours.at(i).x){
      maxx = contours.at(i).x;
    }
    if(maxy < contours.at(i).y){
      maxy = contours.at(i).y;
    }
  }
  return Point(maxx, maxy);
}
void ipr_threshold(
    Mat_<uchar> &src,
    Mat_<uchar> &dst,
    unsigned char threshold,
    unsigned char max_value)
{
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            if(src(y,x) > threshold){
                dst(y,x) = max_value;
            }else{
                dst(y,x) = 0;
            }
        }
    }
}
unsigned char ipr_threshold_otsu(
    Mat_<uchar> &src,
    Mat_<uchar> &dst,
    unsigned char max_value)
{
    Mat_<float> hist(NBIN, 1, 0.0);
    ipr_calc_histogram(src, hist);
    unsigned char threshold = 1;
    float max_ratio = 0;
    for(int low_number = 1; low_number < NBIN ; low_number++) // しきい値を動かし、クラス間分散/クラス内分散の最大値を探す。
    {
        int high_number = NBIN - low_number;

        int low_sum = 0;               //クラス1の濃度値の合計
        int high_sum = 0;              //クラス2の濃度値の合計
        int low_pixel_number = 0;      //クラス1の画素数
        int high_pixel_number = 0;     //クラス2の画素数
        for(int i = 0; i < NBIN; i++){
            if( i < low_number ){
                low_sum += i * hist(i,0);
                low_pixel_number += hist(i,0);
            }else{
                high_sum += i * hist(i,0);
                high_pixel_number += hist(i,0);
            }
        }
        //cout << format("%d %d %d %d %d \n", low_number, low_sum, high_sum,low_pixel_number,high_pixel_number);
        int sum = low_sum + high_sum;
        int pixel_number = low_pixel_number + high_pixel_number;

        float average = sum / (float)pixel_number;
        float low_average = low_sum / (float)low_pixel_number;
        float high_average = high_sum / (float)high_pixel_number;
        // cout << format("%f %f %f \n", average, low_average, high_average);

        float sum1 = 0; //分散を計算する途中の総和の部分
        float sum2 = 0;
        for(int i = 0; i < low_number; i++){
            sum1 += hist(i,0) * pow( i - low_average , 2);
        }
        for(int i = low_number; i < max_value; i++){
            sum2 += hist(i,0) * pow( i - high_average , 2);
        }
	    float low_variance = sum1 / low_pixel_number;
	    float high_variance = sum2 / high_pixel_number;//分散

	    //クラス内分散
	    float inside_variance = (low_variance + high_variance) / pixel_number;

	    //クラス間分散	
 	    float between_variance = (low_pixel_number * pow(low_average - average, 2) + high_pixel_number * pow(high_average - average, 2)) / pixel_number;
        float local_ratio = between_variance / inside_variance;
        if( local_ratio > max_ratio){
            max_ratio = local_ratio;
            threshold = low_number;
        }
    } 

    ipr_threshold(src, dst, threshold, max_value);

    return threshold;
}
void ipr_erode(
  Mat_<uchar>& src,
  Mat_<uchar>& dst,
  int iteration
)
{
  Mat_<uchar> tmp(src.rows + 2, src.cols + 2);
  // dst = src.clone();
  for(int i = 0; i < iteration; i++){
    //四隅  
    //(0,0)
    tmp(0,0) = dst(0,0);
    tmp(1,1) = dst(0,0);
    tmp(0,1) = dst(0,0);
    tmp(1,0) = dst(0,0);

    //(0,tmp.cols - 1)
    tmp(0,tmp.cols-1) = dst(0,dst.cols-1);
    tmp(0,tmp.cols-2) = dst(0,dst.cols-1);
    tmp(1,tmp.cols-1) = dst(0,dst.cols-1);
    tmp(1,tmp.cols-2) = dst(0,dst.cols-1);

    //(tmp.rows-1, 0)
    tmp(tmp.rows-1,0) = dst(dst.rows-1,0);
    tmp(tmp.rows-1,1) = dst(dst.rows-1,0);
    tmp(tmp.rows-2,0) = dst(dst.rows-1,0);
    tmp(tmp.rows-2,1) = dst(dst.rows-1,0);

    //(tmp.rows-1,tmp.cols-1)
    tmp(tmp.rows-1,tmp.cols-1) = dst(dst.rows-1,dst.cols-1);
    tmp(tmp.rows-2,tmp.cols-1) = dst(dst.rows-1,dst.cols-1);
    tmp(tmp.rows-1,tmp.cols-2) = dst(dst.rows-1,dst.cols-1);
    tmp(tmp.rows-2,tmp.cols-2) = dst(dst.rows-1,dst.cols-1);
    
    //上
    for(int x = 1; x < dst.cols - 1; x++){
      tmp(0,x+1) = dst(0,x);
    }

    //下
    for(int x = 1; x < dst.cols - 1; x++){
      tmp(tmp.rows-1,x+1) = dst(dst.rows-1,x);
    }

    //左
    for(int y = 1; y < dst.rows - 1; y++){
      tmp(y+1,0) = dst(y,0);
    }

    //右
    for(int y = 1; y < dst.rows - 1; y++){
      tmp(y+1,tmp.cols-1) = dst(y,dst.cols-1);
    }

    //中
    for(int y = 0 ; y < dst.rows; y++){
      for(int x = 0 ; x < dst.cols; x++){
        tmp(y+1,x+1) = dst(y,x);
      }
    }
    for (int y = 1; y < tmp.rows - 1; y++)
    {
      for (int x = 1; x < tmp.cols - 1; x++)
      {
        if (tmp(y - 1,x - 1) == 0 || tmp(y - 1,x) == 0 || tmp(y - 1,x + 1) == 0 ||
            tmp(y    ,x - 1) == 0 || tmp(y    ,x) == 0 || tmp(y    ,x + 1) == 0 ||
            tmp(y + 1,x - 1) == 0 || tmp(y + 1,x) == 0 || tmp(y + 1,x + 1) == 0)
        {
          dst(y-1,x-1) = 0;
        }
        else
        {
          dst(y-1,x-1) = 255;
        }
      }
    }
  }
}
void ipr_dilate(
  Mat_<uchar>& src,
  Mat_<uchar>& dst,
  int iteration
)
{
  Mat_<uchar> tmp(src.rows + 2, src.cols + 2);
  dst = src.clone();
  for(int i = 0; i < iteration; i++){
    //四隅  
    //(0,0)
    tmp(0,0) = dst(0,0);
    tmp(1,1) = dst(0,0);
    tmp(0,1) = dst(0,0);
    tmp(1,0) = dst(0,0);

    //(0,tmp.cols - 1)
    tmp(0,tmp.cols-1) = dst(0,dst.cols-1);
    tmp(0,tmp.cols-2) = dst(0,dst.cols-1);
    tmp(1,tmp.cols-1) = dst(0,dst.cols-1);
    tmp(1,tmp.cols-2) = dst(0,dst.cols-1);

    //(tmp.rows-1, 0)
    tmp(tmp.rows-1,0) = dst(dst.rows-1,0);
    tmp(tmp.rows-1,1) = dst(dst.rows-1,0);
    tmp(tmp.rows-2,0) = dst(dst.rows-1,0);
    tmp(tmp.rows-2,1) = dst(dst.rows-1,0);

    //(tmp.rows-1,tmp.cols-1)
    tmp(tmp.rows-1,tmp.cols-1) = dst(dst.rows-1,dst.cols-1);
    tmp(tmp.rows-2,tmp.cols-1) = dst(dst.rows-1,dst.cols-1);
    tmp(tmp.rows-1,tmp.cols-2) = dst(dst.rows-1,dst.cols-1);
    tmp(tmp.rows-2,tmp.cols-2) = dst(dst.rows-1,dst.cols-1);
    
    //上
    for(int x = 1; x < dst.cols - 1; x++){
      tmp(0,x+1) = dst(0,x);
    }

    //下
    for(int x = 1; x < dst.cols - 1; x++){
      tmp(tmp.rows-1,x+1) = dst(dst.rows-1,x);
    }

    //左
    for(int y = 1; y < dst.rows - 1; y++){
      tmp(y+1,0) = dst(y,0);
    }

    //右
    for(int y = 1; y < dst.rows - 1; y++){
      tmp(y+1,tmp.cols-1) = dst(y,dst.cols-1);
    }

    //中
    for(int y = 0 ; y < dst.rows; y++){
      for(int x = 0 ; x < dst.cols; x++){
        tmp(y+1,x+1) = dst(y,x);
      }
    }
    for (int y = 1; y < tmp.rows - 1; y++)
    {
      for (int x = 1; x < tmp.cols - 1; x++)
      {
        if (tmp(y - 1,x - 1) != 0 || tmp(y - 1,x) != 0 || tmp(y - 1,x + 1) != 0 ||
            tmp(y    ,x - 1) != 0 || tmp(y    ,x) != 0 || tmp(y    ,x + 1) != 0 ||
            tmp(y + 1,x - 1) != 0 || tmp(y + 1,x) != 0 || tmp(y + 1,x + 1) != 0)
        {
          dst(y-1,x-1) = 255;
        }
        else
        {
          dst(y-1,x-1) = 0;
        }
      }
    }
  }
}


