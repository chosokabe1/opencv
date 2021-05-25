#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat colorImage = imread(argv[1], IMREAD_COLOR);
    if ( colorImage.empty() ) {
        cerr << format("can't %s open.\n", argv[1]);
        exit(1);
    }

    Mat grayImage;
    cvtColor(colorImage, grayImage, COLOR_BGR2GRAY);

    Mat npImage;
    bitwise_not(grayImage, npImage);

    imshow("nega-posi", npImage);
    waitKey(0);

    return 0;
}
