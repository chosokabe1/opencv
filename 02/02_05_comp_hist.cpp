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

const int COMP_CORREL = 1;
const int COMP_CHISQR = 2;
const int COMP_INTERSECT = 3;
const int COMP_BHATTACHARYYA = 4;
const int COMP_CORREL_TEST = 5;

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
int ipr_calc_mode(Mat &hist){
    int mode = 0;
    for(int i = 0; i < hist.rows; i++ ){
       if(hist.at<double>(i,0) > hist.at<double>(mode,0)){
           mode = i;
       }
    }
    return mode;
}
double ipr_compare_histogram(Mat &hist1, Mat &hist2, int method){
    double similarity = 0;
    int mode1 = ipr_calc_mode(hist1);
    int mode2 = ipr_calc_mode(hist2);

    if(method == COMP_CORREL){
        double sum1 = 0;
        double sum2 = 0;
        double sum3 = 0;
        for(int i = 0; i < hist1.rows; i++ ){
            sum1 += (hist1.at<double>(i,0) - mode1) * (hist2.at<double>(i,0) - mode2);
            sum2 += pow((hist1.at<double>(i,0) - mode1), 2);
            sum3 += pow((hist2.at<double>(i,0) - mode2), 2);
        }
        similarity = sum1 / sqrt(sum2 * sum3);
    }
    if(method == COMP_CHISQR){
        for(int i = 0; i < hist1.rows; i++){
            if(hist1.at<double>(i,0) + hist2.at<double>(i,0) == 0){
                continue;
            }
            similarity += pow(hist1.at<double>(i,0) - hist2.at<double>(i,0), 2) / (hist1.at<double>(i,0) + hist2.at<double>(i,0));
        }
    }
    if(method == COMP_INTERSECT){
        for(int i = 0; i < hist1.rows; i++){
            if(hist1.at<double>(i,0) > hist2.at<double>(i,0)){
                similarity += hist2.at<double>(i,0);
            }else{
                similarity += hist1.at<double>(i,0);
            }
        }
    }
    if(method == COMP_BHATTACHARYYA){
        int sum = 0;
        for(int i = 0; i < hist1.rows; i++){
            sum += sqrt(hist1.at<double>(i,0) * hist2.at<double>(i,0));
        }
        similarity = sqrt(1 - 1 / sqrt(mode1 * mode2 * pow(hist1.rows, 2)));
    }
    return similarity;
}
int main(int argc, const char* argv[])
{
    
    Mat_<uchar> src1 = imread("lenna.pgm", IMREAD_GRAYSCALE);
    if(src1.empty()){
        cerr << "can't image1 open." << endl;
        exit(1);
    }
    Mat_<uchar> src2 = imread("baboon.pgm", IMREAD_GRAYSCALE);
    if(src2.empty()){
        cerr << "can't image2 open." << endl;
        exit(1);
    }
    Mat_<uchar> src3 = imread("pepper.pgm", IMREAD_GRAYSCALE);
    if(src3.empty()){
        cerr << "can't image3 open." << endl;
        exit(1);
    }

    int elemSize_bit = src1.elemSize() * 8;
    int tone_number = pow(2, elemSize_bit);

    Mat hist1 = Mat::zeros(tone_number,1,CV_64F);
    Mat hist2 = Mat::zeros(tone_number,1,CV_64F);
    Mat hist3 = Mat::zeros(tone_number,1,CV_64F);

    Mat normalize_hist1 = Mat::zeros(tone_number, 1, CV_64F);
    Mat normalize_hist2 = Mat::zeros(tone_number, 1, CV_64F);
    Mat normalize_hist3 = Mat::zeros(tone_number, 1, CV_64F);

    ipr_calc_histgram(src1, hist1);
    ipr_calc_histgram(src2, hist2);
    ipr_calc_histgram(src3, hist3);

    ipr_normalize_histogram(hist1, normalize_hist1);
    ipr_normalize_histogram(hist2, normalize_hist2);
    ipr_normalize_histogram(hist3, normalize_hist3);



    cout << "similarity_1_2 = " << ipr_compare_histogram(normalize_hist1,normalize_hist2,COMP_CORREL) << endl;
    cout << "similarity_1_3 = " << ipr_compare_histogram(normalize_hist1,normalize_hist3,COMP_CORREL) << endl;
    cout << "similarity_2_3 = " << ipr_compare_histogram(normalize_hist2,normalize_hist2,COMP_CORREL) << endl;
    cout << "CHI_similarity_1_2 = " << ipr_compare_histogram(normalize_hist1,normalize_hist2,COMP_CHISQR) << endl;
    cout << "INTERSECT_similarity_1_2 = " << ipr_compare_histogram(normalize_hist1,normalize_hist2,COMP_INTERSECT) << endl;
    cout << "BHATTACHARYYA_similarity_1_2 = " << ipr_compare_histogram(normalize_hist1,normalize_hist2,COMP_BHATTACHARYYA) << endl;


    


    return 0;
}