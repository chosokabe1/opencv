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
#define TONE_NUM 256

using namespace std;
using namespace cv;

void ipr_calc_histgram(Mat_<uchar> &image, Mat_<double> &hist){
    
    Mat_<uchar> temp(HEIGHT,WIDTH);
    int cnt;
    temp = image.clone();
    for(int i = 0; i < TONE_NUM; i++){
        temp = image.clone();
        inRange(temp,i,i,temp);
        cnt = countNonZero(temp);
        hist(i,0) = cnt;
    }
}
int main(int argc, const char* argv[])
{
    Mat_<double> hist(TONE_NUM, 1);
    for(int i; i< TONE_NUM; i++){
        double tmp = 0;
        hist(i,0) = tmp;
    }
    Mat_<uchar> src1 = imread("lenna.pgm", IMREAD_GRAYSCALE);
    if(src1.empty()){
        cerr << "can't image open." << endl;
        exit(1);
    }

    ipr_calc_histgram(src1, hist);
    const char *fileName = "data.dat";
    ofstream ofs(fileName);
    if(!ofs)
    {
        cout << "ファイルが開けませんでした。" << endl;
        return -1;
    }

    for( int i = 0; i < TONE_NUM; i++ ){
        ofs << i << " " << hist(i,0) << endl;
    }

    return 0;
}