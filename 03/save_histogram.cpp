#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<float> &hist)
{
    // ヒストグラムの算出
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            hist(image(y, x), 0) += 1.0;
        }
    }
}

void ipr_calc_int_image_histogram(const Mat_<int> &image, Mat_<float> &hist)
{
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            hist(image(y, x), 0) += 1.0;
        }
    }
}
void ipr_save_histogram(const Mat_<float> &hist, const char path[])
{
    ofstream ofs(path);

    for (int n = 0; n < hist.rows; n++) {
        ofs << format("%d %f\n", n, hist(n, 0));
    }
}
void ipr_save_int_image_histogram(const Mat_<float> &hist, const char path[])
{
    ofstream ofs(path);

    for (int n = 0; n < hist.rows; n++) {
        ofs << format("%d %f\n", n, hist(n, 0));
    }
}