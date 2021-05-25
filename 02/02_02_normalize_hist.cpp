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

#define HEIGHT 256
#define WIDTH 256

using namespace std;
using namespace cv;

void ipr_calc_histgram(Mat_<uchar> &image, Mat &hist){
    
    Mat_<uchar> temp(HEIGHT,WIDTH);
    int cnt;
    int elemSize_bit = image.elemSize() * 8;
    int tone_number = pow(2, elemSize_bit);
    temp = image.clone();
    for(int i = 0; i < tone_number; i++){
        temp = image.clone();
        inRange(temp,i,i,temp);
        cnt = countNonZero(temp);
        hist.at<double>(i,0) = cnt;
    }
}
void ipr_normalize_histogram(Mat &src, Mat &dst){
    double tone_number = (double)src.rows;
    double area = pow(tone_number, 2);
    dst = src / area;
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
    const char *fileName = "data.dat";
    const char *normalize_fileName = "normalize_data.dat";
    ofstream ofs(fileName);
    if(!ofs)
    {
        cout << "ファイルが開けませんでした。" << endl;
        return -1;
    }
    for( int i = 0; i < tone_number; i++ ){
        ofs << i << " " << hist.at<double>(i,0) << endl;
    }
    ofstream normalize_ofs(normalize_fileName);
    if(!normalize_ofs)
    {
        cout << "ファイルが開けませんでした。" << endl;
        return -1;
    }
    for( int i = 0; i < tone_number; i++ ){
        normalize_ofs << i << " " << normalize_hist.at<double>(i,0) << endl;
    }



    return 0;
}