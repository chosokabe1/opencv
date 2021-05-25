// #include <opencv2/core/utility.hpp>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, const char* argv[])
{
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);
    if(img.empty()){
        cerr << "can't image open." << endl;
        exit(1);
    }
    imshow("test",  img);
    waitKey(0);
    Mat color_src;
    cvtColor(img,color_src,COLOR_GRAY2BGR);
    imwrite(argv[2], color_src);

    return 0;
}