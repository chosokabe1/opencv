#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

enum {
    IPR_MORPH_OPEN = 0,
    IPR_MORPH_CLOSE
};

void ipr_erode(Mat_<uchar>& src, Mat_<uchar>& dst, int iteration);
void ipr_dilate(Mat_<uchar>& src, Mat_<uchar>& dst, int iteration);
void ipr_morphologyEx(Mat_<uchar>& src, Mat_<uchar>& dst, int op, int iteration);


int main(int argc, char *argv[])
{
    Mat_<uchar> src = imread(argv[1], IMREAD_GRAYSCALE);

    Mat_<uchar> binary;
    threshold(src, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

    Mat_<uchar> dst;
    ipr_morphologyEx(binary, dst, IPR_MORPH_CLOSE, 1);
    ipr_morphologyEx(dst, dst, IPR_MORPH_OPEN, 1);

    imshow("SOURCE BINARY IMAGE", binary);
    imshow("DILATED IMAGE", dst);
    
    waitKey(0);

    return 0;
}

void ipr_erode(Mat_<uchar>& src, Mat_<uchar>& dst, int iteration)
{
    // 置換モードでない場合は，出力画像の領域を確保
    if ( dst.empty() ) {
        dst = Mat_<uchar>(src.size(), (uchar) 0);
    }
    
    // 処理を反復しても原画像を破壊しないために複製
    Mat_<uchar> tmp = src.clone();

    for (int i = 0; i < iteration; i++) {

        /////
        // 四隅の処理
        /////
        // 左上の処理
        if (tmp(0, 0) == 0 || tmp(0, 1) == 0 ||
            tmp(1, 0) == 0 || tmp(1, 1) == 0 ) {
            dst(0, 0) = 0;
        } else {
            dst(0, 0) = 255;
        }

        // 右上の処理
        if (tmp(0, tmp.cols - 2) == 0 || tmp(0, tmp.cols - 1) == 0 ||
            tmp(1, tmp.cols - 2) == 0 || tmp(1, tmp.cols - 1) == 0 ) {
            dst(0, tmp.cols - 1) = 0;
        } else {
            dst(0, tmp.cols - 1) = 255;
        }

        // 左下の処理
        if (tmp(tmp.rows - 2, 0) == 0 || tmp(tmp.rows - 2, 1) == 0 ||
            tmp(tmp.rows - 1, 0) == 0 || tmp(tmp.rows - 1, 1) == 0 ) {
            dst(tmp.rows - 1, 0) = 0;
        } else {
            dst(tmp.rows - 1, 0) = 255;
        }

        // 右下の処理
        if (tmp(tmp.rows - 2, tmp.cols - 2) == 0 || tmp(tmp.rows - 2, tmp.cols - 1) == 0 ||
            tmp(tmp.rows - 1, tmp.cols - 2) == 0 || tmp(tmp.rows - 1, tmp.cols - 1) == 0 ) {
            dst(tmp.rows - 1, tmp.cols - 1) = 0;
        } else {
            dst(tmp.rows - 1, tmp.cols - 1) = 255;
        }


        /////
        // 四隅を除く端の処理
        /////
        // 上端と下端の処理
        for (int x = 1; x < tmp.cols - 1; x++) {
            int y = 0;
            if (tmp(y,     x - 1) == 0 || tmp(y,     x) == 0 || tmp(y,     x + 1) == 0 ||
                tmp(y + 1, x - 1) == 0 || tmp(y + 1, x) == 0 || tmp(y + 1, x + 1) == 0 ) {
                dst(y, x) = 0;
            } else {
                dst(y, x) = 255;
            }

            y = tmp.rows - 1;
            if (tmp(y - 1, x - 1) == 0 || tmp(y - 1, x) == 0 || tmp(y - 1, x + 1) == 0 ||
                tmp(y    , x - 1) == 0 || tmp(y    , x) == 0 || tmp(y    , x + 1) == 0 ) {
                dst(y, x) = 0;
            } else {
                dst(y, x) = 255;
            }
        }

        // 右端と左端の処理
        for (int y = 1; y < tmp.rows - 1; y++) {
            int x = 0;
            if (tmp(y - 1, x) == 0 || tmp(y - 1, x + 1) == 0 ||
                tmp(y    , x) == 0 || tmp(y    , x + 1) == 0 ||
                tmp(y + 1, x) == 0 || tmp(y + 1, x + 1) == 0 ) {
                dst(y, x) = 0;
            } else {
                dst(y, x) = 255;
            }

            x = tmp.cols - 1; 
            if (tmp(y - 1, x - 1) == 0 || tmp(y - 1, x) == 0 ||
                tmp(y    , x - 1) == 0 || tmp(y    , x) == 0 ||
                tmp(y + 1, x - 1) == 0 || tmp(y + 1, x) == 0 ) {
                dst(y, x) = 0;
            } else {
                dst(y, x) = 255;
            }
        }

        /////
        // 中の処理
        /////
        for (int y = 1; y < tmp.rows - 1; y++) {
            for (int x = 1; x < tmp.cols - 1; x++) {
                if (tmp(y - 1, x - 1) == 0 || tmp(y - 1, x) == 0 || tmp(y - 1, x + 1) == 0 ||
                    tmp(y    , x - 1) == 0 || tmp(y,     x) == 0 || tmp(y    , x + 1) == 0 ||
                    tmp(y + 1, x - 1) == 0 || tmp(y + 1, x) == 0 || tmp(y + 1, x + 1) == 0) {
                    dst(y, x) = 0;
                } else {
                    dst(y, x) = 255;
                }
            }
        }

        tmp = dst.clone();
    }
}

void ipr_dilate(Mat_<uchar>& src, Mat_<uchar>& dst, int iteration)
{
    // 置換モードでない場合は，出力画像の領域を確保
    if ( dst.empty() ) {
        dst = Mat_<uchar>(src.size(), (uchar) 0);
    }
    
    // 処理を反復しても原画像を破壊しないために複製
    Mat_<uchar> tmp = src.clone();

    for (int i = 0; i < iteration; i++) {

        /////
        // 四隅の処理
        /////
        // 左上の処理
        if (tmp(0, 0) != 0 || tmp(0, 1) != 0 ||
            tmp(1, 0) != 0 || tmp(1, 1) != 0 ) {
            dst(0, 0) = 255;
        } else {
            dst(0, 0) = 0;
        }

        // 右上の処理
        if (tmp(0, tmp.cols - 2) != 0 || tmp(0, tmp.cols - 1) != 0 ||
            tmp(1, tmp.cols - 2) != 0 || tmp(1, tmp.cols - 1) != 0 ) {
            dst(0, tmp.cols - 1) = 255;
        } else {
            dst(0, tmp.cols - 1) = 0;
        }

        // 左下の処理
        if (tmp(tmp.rows - 2, 0) != 0 || tmp(tmp.rows - 2, 1) != 0 ||
            tmp(tmp.rows - 1, 0) != 0 || tmp(tmp.rows - 1, 1) != 0 ) {
            dst(tmp.rows - 1, 0) = 255;
        } else {
            dst(tmp.rows - 1, 0) = 0;
        }

        // 右下の処理
        if (tmp(tmp.rows - 2, tmp.cols - 2) != 0 || tmp(tmp.rows - 2, tmp.cols - 1) != 0 ||
            tmp(tmp.rows - 1, tmp.cols - 2) != 0 || tmp(tmp.rows - 1, tmp.cols - 1) != 0 ) {
            dst(tmp.rows - 1, tmp.cols - 1) = 255;
        } else {
            dst(tmp.rows - 1, tmp.cols - 1) = 0;
        }


        /////
        // 四隅を除く端の処理
        /////
        // 上端と下端の処理
        for (int x = 1; x < tmp.cols - 1; x++) {
            int y = 0;
            if (tmp(y,     x - 1) != 0 || tmp(y,     x) != 0 || tmp(y,     x + 1) != 0 ||
                tmp(y + 1, x - 1) != 0 || tmp(y + 1, x) != 0 || tmp(y + 1, x + 1) != 0 ) {
                dst(y, x) = 255;
            } else {
                dst(y, x) = 0;
            }

            y = tmp.rows - 1;
            if (tmp(y - 1, x - 1) != 0 || tmp(y - 1, x) != 0 || tmp(y - 1, x + 1) != 0 ||
                tmp(y    , x - 1) != 0 || tmp(y    , x) != 0 || tmp(y    , x + 1) != 0 ) {
                dst(y, x) = 255;
            } else {
                dst(y, x) = 0;
            }
        }

        // 右端と左端の処理
        for (int y = 1; y < tmp.rows - 1; y++) {
            int x = 0;
            if (tmp(y - 1, x) != 0 || tmp(y - 1, x + 1) != 0 ||
                tmp(y    , x) != 0 || tmp(y    , x + 1) != 0 ||
                tmp(y + 1, x) != 0 || tmp(y + 1, x + 1) != 0 ) {
                dst(y, x) = 255;
            } else {
                dst(y, x) = 0;
            }

            x = tmp.cols - 1; 
            if (tmp(y - 1, x - 1) != 0 || tmp(y - 1, x) != 0 ||
                tmp(y    , x - 1) != 0 || tmp(y    , x) != 0 ||
                tmp(y + 1, x - 1) != 0 || tmp(y + 1, x) != 0 ) {
                dst(y, x) = 255;
            } else {
                dst(y, x) = 0;
            }
        }

        /////
        // 中の処理
        /////
        for (int y = 1; y < tmp.rows - 1; y++) {
            for (int x = 1; x < tmp.cols - 1; x++) {
                if (tmp(y - 1, x - 1) != 0 || tmp(y - 1, x) != 0 || tmp(y - 1, x + 1) != 0 ||
                    tmp(y    , x - 1) != 0 || tmp(y,     x) != 0 || tmp(y    , x + 1) != 0 ||
                    tmp(y + 1, x - 1) != 0 || tmp(y + 1, x) != 0 || tmp(y + 1, x + 1) != 0) {
                    dst(y, x) = 255;
                } else {
                    dst(y, x) = 0;
                }
            }
        }

        tmp = dst.clone();
    }
}

void ipr_morphologyEx(Mat_<uchar>& src, Mat_<uchar>& dst, int op, int iteration)
{
    if (op == IPR_MORPH_CLOSE) {
        ipr_dilate(src, dst, iteration);
        ipr_erode(dst, dst, iteration);    
    } else if (op == IPR_MORPH_OPEN) {
        ipr_erode(src, dst, iteration);
        ipr_dilate(dst, dst, iteration);
    } else {
        cerr << format("IPR_MORPH_CLOSE か IPR_MORPH_OPEN を指定して下さい．\n");
        exit(1); 
    }
}