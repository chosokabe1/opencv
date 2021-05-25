#include <iostream>
#include <vector>
#include <cstdlib>
#include <opencv2/opencv.hpp>

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<double> &hist);
void ipr_normalize_histogram(const Mat_<double> &src, Mat_<double> &dst);

int main(int argc, char *argv[])
{
    Mat_<uchar> image = imread(argv[1], IMREAD_GRAYSCALE);
    if ( image.empty() ) {
        cerr << format("can't image (%s) open.\n", argv[1]);
        exit(1);
    }
    /////
    // format は OpenCV で用意された関数です．
    // 色々なスタイルで Mat を出力してくれます．
    // printf のような使い方など，色々便利です．
    /////

    // ヒストグラムの領域を確保して，ゼロで初期化
    Mat_<double> hist(NBIN, 1, 0.0);
    ipr_calc_histogram(image, hist);

    vector<Mat_<double>> dstHist(3);

    // ヒストグラムの和が 1 になるように正規化
    dstHist[0] = Mat_<double>(NBIN, 1, 0.0);  // 自作関数を使うので，領域の確保が必要
    ipr_normalize_histogram(hist, dstHist[0]);

    // OpenCV の normalize 関数を使う場合
    normalize(hist, dstHist[1], 1, 0, NORM_L1);

    // Mat クラスのメソッド convertScale を使う
    Scalar total = sum(hist);
    hist.convertTo(dstHist[2], -1, 1.0 / total.val[0], 0);

    // もっと簡単に，以下でも OK
    dstHist[2] = hist / total.val[0];

    // 画面に結果を表示
    //   ファイルに保存したい場合は，リダイレクト >> を使えばできる
    //   もしくは，ファイル操作で保存しても OK
    for (int n = 0; n < hist.rows; n++) {
        // cout << format("%d %f\n", n, hist(n, 0));
    }

    // cout << dstHist[0] << endl;

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

void ipr_normalize_histogram(const Mat_<double> &src, Mat_<double> &dst)
{
    double total = 0;
    for (int n = 0; n < src.rows; n++) {
        total += src(n, 0);
    }
    cout << "sum = " << total << endl; 

    for (int n = 0; n < src.rows; n++) {
        dst(n, 0) = src(n, 0) / total;
    }
}
