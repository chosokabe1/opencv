#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<float> &hist)
{
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            hist(image(y, x), 0) += 1.0;
        }
    }
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

int main(int argc, char *argv[])
{
    const unsigned char max_value = 255;
    // int threshold_int = atoi(argv[2]);
    // const unsigned char threshold = (unsigned char)threshold_int;
    Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
    Mat_<uchar> dst(src.rows, src.cols);
    int threshold = (int)ipr_threshold_otsu(src,dst,max_value);
    cout << "threshold = " << threshold << endl;
    imshow("2値か", dst);
    waitKey(0);
    
    return 0;
}
