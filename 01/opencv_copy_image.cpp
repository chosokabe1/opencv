#include <opencv2/core/utility.hpp>
#include <string>
#include <opencv2/opencv.hpp>
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

    std::string pgm = "pgm";
    std::string argv1 = argv[1];
    std::size_t found = argv1.find(pgm);
    if( found != std::string::npos){
        cvtColor(img,img,COLOR_RGB2GRAY);
    }
    imwrite(argv[2], img);

    return 0;
}