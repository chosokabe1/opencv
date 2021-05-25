#include <opencv2/core/utility.hpp>
#include <opencv2/core/fast_math.hpp>
#include <opencv2/core/softfloat.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#define HEIGHT 256
#define WIDTH 256

using namespace std;
using namespace cv;


int main(int argc, const char* argv[])
{
   
    double temp;
    Mat mtx(HEIGHT, WIDTH, CV_64F);
    for(double i = 0; i < HEIGHT; i++){
        for(double j = i; j < WIDTH; j++){
            temp = sqrt(pow(i,2) + pow(j,2)) / sqrt(pow(HEIGHT-1,2) + pow(WIDTH-1,2));
            mtx.at<double>(i,j) = temp;
            mtx.at<double>(j,i) = temp;
        }
    }
    
    Mat img = imread(argv[1], IMREAD_COLOR);
    if(img.empty()){
        cerr << "can't image open." << endl;
        exit(1);
    }
    Mat img2 = imread(argv[2], IMREAD_COLOR);
    if(img2.empty()){
        cerr << "can't image open." << endl;
        exit(1);
    }

    Mat img_f;
    img.convertTo(img_f,CV_64F);
    Mat channels[3];
    split(img_f,channels);
    channels[0] = channels[0].mul(mtx);
    channels[1] = channels[1].mul(mtx);
    channels[2] = channels[2].mul(mtx);
    merge(channels,3,img_f);
    img_f.convertTo(img,CV_8U);

    img2.convertTo(img_f,CV_64F);
    split(img_f,channels);
    channels[0] = channels[0].mul(mtx);
    channels[1] = channels[1].mul(mtx);
    channels[2] = channels[2].mul(mtx);
    merge(channels,3,img_f);
    img_f.convertTo(img2,CV_8U);

    img += img2;
   
    imshow("test",  img);
    waitKey(0);
    return 0;
}