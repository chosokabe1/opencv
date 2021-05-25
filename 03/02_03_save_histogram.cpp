#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const int NBIN = 256;

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<double> &hist);
void ipr_save_histogram(const Mat_<double> &hist, const char path[]);

int main(int argc, char *argv[])
{
    // 引数の数をチェック
    if (argc != 3) {
        cerr << format("Usage: %s path_to_image dst_file\n", argv[0]);
        exit(1);
    }

    Mat_<uchar> image = imread(argv[1], IMREAD_GRAYSCALE);

    Mat_<double> hist(NBIN, 1, 0.0);
    ipr_calc_histogram(image, hist);

    ipr_save_histogram(hist, argv[2]);

    return 0;
}

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<double> &hist)
{
    // ヒストグラムの算出
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            hist(image(y, x), 0) += 1.0;
        }
    }
}

void ipr_save_histogram(const Mat_<double> &hist, const char path[])
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
