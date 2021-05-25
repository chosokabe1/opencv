#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat_<uchar> gray = imread(argv[1], IMREAD_GRAYSCALE);

    Mat_<uchar> binary;
    threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    Mat_<uchar> open_close;
    morphologyEx(binary, open_close, MORPH_OPEN, Mat());
    morphologyEx(open_close, open_close, MORPH_CLOSE, Mat());

    Mat_<int> labels, stats;
    Mat_<double> centroids;
    int nLabels = connectedComponentsWithStats(open_close, labels, stats, centroids);

    Mat_<uchar> mask(gray.size(), 0);
    vector<Rect> region;
    for (int l = 1; l < nLabels; l++) {
        int *params = stats.ptr<int>(l);

        int x = params[ConnectedComponentsTypes::CC_STAT_LEFT];
        int y = params[ConnectedComponentsTypes::CC_STAT_TOP];
        int h = params[ConnectedComponentsTypes::CC_STAT_HEIGHT];
        int w = params[ConnectedComponentsTypes::CC_STAT_WIDTH];
        float aspect_ratio = h / static_cast<float>(w);
        float height_ratio = h / static_cast<float>(gray.rows);
        cout << format("%2d %5d %4.2f %4.2f\n", l, params[ConnectedComponentsTypes::CC_STAT_AREA], aspect_ratio, height_ratio);
        
        if ( (1.3 < aspect_ratio && aspect_ratio < 2.3) &&
             (0.7 < height_ratio && height_ratio < 0.9) ) {
            region.push_back(Rect(x, y, w, h));

            Mat_<uchar> tmp;
            compare(labels, l, tmp, CMP_EQ);
            bitwise_or(tmp, mask, mask);
        }
    }

    Mat_<Vec3b> gray_3ch;
    cvtColor(gray, gray_3ch, COLOR_GRAY2BGR);

    Mat_<Vec3b> red = gray_3ch.clone();
    red.setTo(CV_RGB(255, 80, 80), mask);

    Mat_<Vec3b> result = gray_3ch.clone();
    result = 0.5 * result + 0.5 * red;
    for (auto r : region) {
        rectangle(result, r, CV_RGB(0, 113, 188), 3);
    }

    Mat_<Vec3b> binary_3ch;
    cvtColor(binary, binary_3ch, COLOR_GRAY2BGR);

    Mat_<Vec3b> open_close_3ch;
    cvtColor(open_close, open_close_3ch, COLOR_GRAY2BGR);

    Mat_<Vec3b> mask_3ch;
    cvtColor(mask, mask_3ch, COLOR_GRAY2BGR);

    Mat_<Vec3b> dst;
    vconcat(vector<Mat>{gray_3ch, binary_3ch, open_close_3ch, mask_3ch, result}, dst);

    imshow("dst", dst);
    imwrite("result.png", dst);
    waitKey(0);

    return 0;
}
