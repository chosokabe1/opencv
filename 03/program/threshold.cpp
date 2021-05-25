#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;

void ipr_threshold(Mat_<uchar>& src, Mat_<uchar>& dst, uchar threshold, uchar max_value);

int main(int argc, char *argv[])
{
    Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
    Mat_<uchar> dst1(src.size(), (uchar) 0);
    Mat_<uchar> dst2(src.size(), (uchar) 0);
    uchar threshold = atoi(argv[2]);

    ipr_threshold(src, dst1, threshold, 255);
    cv::threshold(src, dst2, threshold, 255, THRESH_BINARY);
    // 第５引数の意味は，http://opencv.jp/cookbook/opencv_img.html#image-binarize を参照
    
    imshow("source image", src);
    imshow("binary image (self made)", dst1);
    imshow("binary image (OpenCV)", dst2);

    waitKey(0);

    return 0;
}

void ipr_threshold(Mat_<uchar>& src, Mat_<uchar>& dst, uchar threshold, uchar max_value)
{
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            if (src(y, x) >= threshold) {
                dst(y, x) = max_value;
            }
            else {
                dst(y, x) = 0;
            }
        }
    }
}
