#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat_<Vec3b> src1 = imread(argv[1], IMREAD_COLOR);
    Mat_<Vec3b> src2 = imread(argv[2], IMREAD_COLOR);
    
    Mat_<Vec3f> alpha(src1.size(), Vec3f::all(0.0));
    float max_length = sqrt(alpha.rows * alpha.rows +
                            alpha.cols * alpha.cols);
    for (int y = 0; y < alpha.rows; y++) {
        for (int x = 0; x < alpha.cols; x++) {
            float length = sqrt(x * x + y * y);
            alpha(y, x)[0] = length / max_length;
            alpha(y, x)[1] = length / max_length;
            alpha(y, x)[2] = length / max_length;
        }
    }

    Mat_<Vec3f> dst(src1.size(), Vec3f::all(0));

    Mat_<Vec3f> tmp1, tmp2;
    src1.convertTo(tmp1, CV_32FC3);
    src2.convertTo(tmp2, CV_32FC3);
    dst = tmp1.mul(alpha) + tmp2.mul(Scalar_<float>::all(1.0) - alpha);
    
    Mat_<Vec3b> result;
    dst.convertTo(result, CV_8UC3);
    
    imshow("alpha blend", result);
    waitKey(0);
    
    return 0;
}
