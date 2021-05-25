#include <opencv2/opencv.hpp>
#include <stack>
#include <vector>

#include <cstdlib>

using namespace std;
using namespace cv;

bool isInImage(cv::Point p, cv::Size s) {
    if (p.x >= 0 && p.x < s.width && p.y >= 0 && p.y < s.height) {
        return true;
    }
    return false;
}

void ipr_threshold(Mat_<uchar>& src, Mat_<uchar>& dst, uchar threshold, uchar max_value);
int ipr_labeling(Mat_<uchar>& binary, Mat_<int>& labelImage);

int main(int argc, char *argv[])
{
    Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);
    Mat_<uchar> binary(src.size(), 0);

    uchar threshold = atoi(argv[2]);
    ipr_threshold(src, binary, threshold, 255);

    cv::morphologyEx(binary, binary, MORPH_OPEN, cv::Mat());
    cv::morphologyEx(binary, binary, MORPH_CLOSE, cv::Mat());

    Mat_<int> labelImage;
    int nlabel = ipr_labeling(binary, labelImage);
    
    vector<Vec3b> colorMap(nlabel + 1, Vec3b::all(0));
    for (int n = 1; n < colorMap.size(); n++) {
        colorMap[n] = Vec3b(rand() % 255, rand() % 255, rand() % 255);
    }

    Mat_<Vec3b> dst(labelImage.size(), Vec3b::all(0));
    for (int y = 0; y < dst.rows; y++) {
        for (int x = 0; x < dst.cols; x++) {
            int l = labelImage(y, x);
            dst(y, x) = colorMap[l];
        }
    }
    cv::imshow("", dst);
    cv::waitKey(0);

    cout << nlabel << endl;

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

int ipr_labeling(Mat_<uchar>& binary, Mat_<int>& labelImage)
{
    std::stack<cv::Point> stackPoint;
    std::vector<cv::Point> neighbor = {
        {1, 0}, { 1, -1}, { 0, -1}, {-1, -1},
        {-1, 0}, {-1,  1}, { 0,  1}, { 1,  1}};
    
    int label = 0;
    labelImage = Mat_<int>(binary.size(), 0);

    for (int y = 0; y < binary.rows; y++) {
        for (int x = 0; x < binary.cols; x++) {

            if( binary(y, x) != 0 && labelImage(y, x) == 0) {
                label++;
                labelImage(y, x) = label;
                stackPoint.push(Point(x, y));

                while( !stackPoint.empty() ) {
                    Point p = stackPoint.top();
                    stackPoint.pop();
                    for (int n = 0; n < neighbor.size(); n++) {
                        cv::Point pp = p + neighbor[n];
                        if (isInImage(pp, binary.size()) &&
                            binary(pp) != 0 &&
                            labelImage(pp) == 0) {
                            labelImage(pp) = label;
                            stackPoint.push(pp);    
                        }
                    }
                }

            }

        }
    }

    return label;
}