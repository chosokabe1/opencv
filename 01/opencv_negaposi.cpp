#include <opencv2/core/utility.hpp>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, const char* argv[])
{
    Mat img = imread(argv[1], IMREAD_COLOR);
    if(img.empty()){
        cerr << "can't image open." << endl;
        exit(1);
    }
    cvtColor(img,img,COLOR_RGB2GRAY);
    bitwise_not(img,img);
    imshow("test",  img);
    waitKey(0);

    return 0;
}