#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

uchar ipr_threshold_otsu(Mat_<uchar>& src, Mat_<uchar>& dst, uchar max_value);

int main(int argc, char *argv[])
{
    Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
    Mat_<uchar> dst1(src.size(), (uchar) 0);
    Mat_<uchar> dst2(src.size(), (uchar) 0);

    uchar threshold_value1 = ipr_threshold_otsu(src, dst1, 255);
    uchar threshold_value2 = cv::threshold(src, dst2, 0.0, 255, THRESH_BINARY | THRESH_OTSU);

    cout << format("%d %d\n", threshold_value1, threshold_value2);

    imshow("source image", src);
    imshow("binary image (self made)", dst1);
    imshow("binary image (OpenCV)", dst2);

    waitKey(0);

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

void ipr_normalize_histogram(Mat_<double> &src, Mat_<double> &dst)
{
    double total = 0;
    for (int n = 0; n < src.rows; n++) {
        total += src(n, 0);
    }

    for (int n = 0; n < src.rows; n++) {
        dst(n, 0) /= total;
    }
}

uchar ipr_threshold_otsu(Mat_<uchar>& src, Mat_<uchar>& dst, uchar max_value)
{
    Mat_<double> histogram(256, 1, 0.0);
    ipr_calc_histogram(src, histogram);
    ipr_normalize_histogram(histogram, histogram);

    // しきい値の候補を 0 〜 255 まで変化させて，分離度が最大となる値を算出
    uchar threshold = 0;
    double max_sigma = 0.0;
    for (int d = 0; d < 255; d++) {

        double omega1 = 0;
        double m1 = 0;
        for (int k = 0; k <= d; k++) {
            omega1 += histogram(k, 0);
            m1 += k * histogram(k, 0);
        }
        if (omega1 == 0.0)
            m1 = 0.0;
        else
            m1 /= omega1;

        double omega2 = 0;
        double m2 = 0;
        for (int k = d + 1; k < 256; k++) {
            omega2 += histogram(k, 0);
            m2 += k * histogram(k, 0);
        }
        if (omega2 == 0.0)
            m2 = 0.0;
        else
            m2 /= omega2;

        double sigma = omega1 * omega2 * (m1 - m2) * (m1 - m2);
        if (sigma > max_sigma) {
            max_sigma = sigma;
            threshold = d;
        }
    }

    // 算出した閾値で二値化
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            if (src(y, x) <= threshold) {
                dst(y, x) = 0;
            }
            else {
                dst(y, x) = max_value;
            }
        }
    }
    
    return threshold;
}
