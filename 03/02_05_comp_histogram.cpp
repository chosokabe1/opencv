#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>

const int NBIN = 256;   // ヒストグラムのビン数

using namespace std;
using namespace cv;

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<float> &hist);
void ipr_normalize_histogram(const Mat_<float> &src, Mat_<float> &dst);
float ipr_compare_histogram(const Mat_<float>& hist1, const Mat_<float> hist2, int method);

int main(int argc, char *argv[])
{
    Mat_<uchar> src1 = imread(argv[1], IMREAD_GRAYSCALE);
    Mat_<uchar> src2 = imread(argv[2], IMREAD_GRAYSCALE);

    Mat_<float> hist1(NBIN, 1, 0.0);
    ipr_calc_histogram(src1, hist1);
    ipr_normalize_histogram(hist1, hist1);

    Mat_<float> hist2(NBIN, 1, 0.0);
    ipr_calc_histogram(src2, hist2);
    ipr_normalize_histogram(hist2, hist2);

    cout << format("%.20f\n", ipr_compare_histogram(hist1, hist2, HISTCMP_CORREL));
    cout << format("%.20f\n", compareHist(hist1, hist2, HISTCMP_CORREL));
    cout << endl;
    cout << format("%.20f\n", ipr_compare_histogram(hist1, hist2, HISTCMP_CHISQR));
    cout << format("%.20f\n", compareHist(hist1, hist2, HISTCMP_CHISQR));
    cout << endl;
    cout << format("%.20f\n", ipr_compare_histogram(hist1, hist2, HISTCMP_INTERSECT));
    cout << format("%.20f\n", compareHist(hist1, hist2, HISTCMP_INTERSECT));
    cout << endl;
    cout << format("%.20f\n", ipr_compare_histogram(hist1, hist2, HISTCMP_BHATTACHARYYA));
    cout << format("%.20f\n", compareHist(hist1, hist2, HISTCMP_BHATTACHARYYA));
    
    return 0;
}

void ipr_calc_histogram(const Mat_<uchar> &image, Mat_<float> &hist)
{
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            hist(image(y, x), 0) += 1.0;
        }
    }
}

void ipr_normalize_histogram(const Mat_<float> &src, Mat_<float> &dst)
{
    float total = 0;
    for (int n = 0; n < src.rows; n++) {
        total += src(n, 0);
    }
    

    for (int n = 0; n < src.rows; n++) {
        dst(n, 0) = src(n, 0) / total;
    }
}

// すべての計算を自作するバージョン
float ipr_comp_histogram_correlation(const Mat_<float>& hist1, const Mat_<float> hist2)
{
    double mean1 = 0.0;
    for (int n = 0; n < hist1.rows; n++) {
        mean1 += hist1(n, 0);
    }
    mean1 = mean1 / hist1.rows;

    double mean2 = 0.0;
    for (int n = 0; n < hist2.rows; n++) {
        mean2 += hist1(n, 0);
    }
    mean2 = mean2 / hist2.rows;

    double num = 0.0;
    for (int n = 0; n < hist1.rows; n++) {
        num += (hist1(n, 0) - mean1) * (hist2(n, 0) - mean2);
    }

    double den1 = 0.0;
    for (int n = 0; n < hist1.rows; n++) {
        den1 += (hist1(n, 0) - mean1) * (hist1(n, 0) - mean1);
    }

    double den2 = 0.0;
    for (int n = 0; n < hist2.rows; n++) {
        den2 += (hist2(n, 0) - mean2) * (hist2(n, 0) - mean2);
    }

    double den = sqrt(den1 * den2);

    float correlation = num / den;

    return correlation;
}

float ipr_comp_histogram_chisqr(const Mat_<float>& hist1, const Mat_<float> hist2)
{
    float chi = 0.0;
    for (int n = 0; n < hist1.rows; n++) {
        float num = (hist1(n, 0) - hist2(n, 0)) * (hist1(n, 0) - hist2(n, 0));
        float den = hist1(n, 0); 
        if (fabs(den) > DBL_EPSILON)
            chi += num / den;
    }

    return chi;
}

float ipr_comp_histogram_intersection(const Mat_<float>& hist1, const Mat_<float> hist2)
{
    float intersection = 0.0;
    for (int n = 0; n < hist1.rows; n++) {
        intersection += std::min(hist1(n, 0), hist2(n, 0));
    }

    return intersection;
}

float ipr_comp_histogram_battacharyya(const Mat_<float>& hist1, const Mat_<float> hist2)
{
    Mat_<float> tmp1(hist1.size(), 0.0);
    Mat_<float> tmp2(hist2.size(), 0.0);
    ipr_normalize_histogram(hist1, tmp1);
    ipr_normalize_histogram(hist2, tmp2);
    
    float bhatt = 0.0;
    for (int n = 0; n < hist1.rows; n++) {
        bhatt += std::sqrt(tmp1(n, 0) * tmp2(n, 0));
    }

    return std::sqrt(1. - bhatt);
}


float ipr_compare_histogram(const Mat_<float>& hist1, const Mat_<float> hist2, int method)
{
    if (method == HISTCMP_CORREL) {
        return ipr_comp_histogram_correlation(hist1, hist2);
    } else if (method == HISTCMP_CHISQR) {
        return ipr_comp_histogram_chisqr(hist1, hist2);
    } else if (method == HISTCMP_INTERSECT) {
        return ipr_comp_histogram_intersection(hist1, hist2);
    } else if (method == HISTCMP_BHATTACHARYYA) {
        return ipr_comp_histogram_battacharyya(hist1, hist2);
    } else {
        cerr << "Unknown comparison method" << endl;
        exit(1);
    }
}