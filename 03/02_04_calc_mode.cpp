#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_calc_histogram(Mat_<uchar> &image, Mat_<double> &hist);
int ipr_calc_mode(Mat_<double> &hist);

int main(int argc, char *argv[])
{
    Mat_<uchar> image = imread(argv[1], IMREAD_GRAYSCALE);

    // ヒストグラムの領域（NBIB 行 1 列の行列）を確保して，ゼロで初期化
    Mat_<double> hist(NBIN, 1, 0.0);
    ipr_calc_histogram(image, hist);

    int mode = ipr_calc_mode(hist);
    cout << mode << endl;

    double min, max;
    Point min_index, max_index;
    minMaxLoc(hist, &min, &max, &min_index, &max_index);
    cout << max_index << endl;

    return 0;
}

void ipr_calc_histogram(Mat_<uchar> &image, Mat_<double> &hist)
{
    // ヒストグラムの算出
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            hist(image(y, x), 0) += 1.0;
        }
    }
}

int ipr_calc_mode(Mat_<double> &hist)
{
    double max = 0.0;
    int mode = 0;
    for (int n = 0; n < hist.rows; n++) {
        if (max < hist(n, 0)) {
            max = hist(n, 0);
            mode = n;
        }
    }

    return mode;
}
