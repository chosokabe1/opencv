#include <opencv2/core/utility.hpp>
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
    resize(img,img,Size(),256.0/img.cols,256.0/img.rows);
    imwrite(argv[2], img);

    return 0;
}