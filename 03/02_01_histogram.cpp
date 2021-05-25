#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<double> &hist);

int main(int argc, char *argv[])
{
    Mat_<uchar> image = imread(argv[1], IMREAD_GRAYSCALE);
    if ( image.empty() ) {
        // cerr:    標準エラー出力（標準出力との違いに注意！）
        // format:  OpenCV の関数で，書式指定表示に便利
        // exit:    強制終了．異常終了の場合は，0 以外の数値を返す
        cerr << format("can't image (%s) open.\n", argv[1]);
        exit(1);
    }

    // ヒストグラムの領域をMatで確保して，ゼロで初期化
    Mat_<double> hist(NBIN, 1, 0.0);
    ipr_calc_histogram(image, hist);

    for (int bin = 0; bin < NBIN; bin++) {
        cout << format("%d %f\n", bin, hist(bin, 0));
    }

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
