#include <opencv2/core/utility.hpp>
#include <opencv2/core/fast_math.hpp>
#include <opencv2/core/softfloat.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void ipr_calc_histgram(Mat_<uchar> &image, Mat &hist){
    for(int i = 0; i < image.rows; i++){
        for(int j= 0; j < image.cols; j++){
            hist.at<double>(image(i,j),0) += 1.0;
        }
    }
}
void ipr_normalize_histogram(Mat &src, Mat &dst){
    double sum = 0;
    for( int i = 0; i < src.rows; i++ ){
        sum += src.at<double>(i,0);
    }
    cout << "sum = " << sum << endl;
    dst = src / sum;
}
int ipr_save_histogram(Mat &hist, const char path[]){
    ofstream ofs(path);
    if(!ofs)
    {
        cerr << "ファイルが開けませんでした。" << endl;
        return -1;
    }
    for( int i = 0; i < hist.rows; i++ ){
        ofs << i << " " << hist.at<double>(i,0) << endl;
    }
    return 0;
}
int main(int argc, const char* argv[])
{
    Mat_<uchar> src1 = imread("lenna.pgm", IMREAD_GRAYSCALE);
    if(src1.empty()){
        cerr << "can't image open." << endl;
        exit(1);
    }
    int elemSize_bit = src1.elemSize() * 8;
    int tone_number = pow(2, elemSize_bit);
    Mat hist = Mat::zeros(tone_number,1,CV_64F);
    Mat normalize_hist = Mat::zeros(tone_number, 1, CV_64F);
    ipr_calc_histgram(src1, hist);
    ipr_normalize_histogram(hist, normalize_hist);
    ipr_save_histogram(hist,"data.dat");
    ipr_save_histogram(normalize_hist,"lenna.dat");
    return 0;
}