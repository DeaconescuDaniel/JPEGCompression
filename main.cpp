#include <iostream>
#include <opencv2/opencv.hpp>
#include "rgbToYCC.h"
#include "ImageBlockMatrix.h"

using namespace cv;



int main() {
    Mat img = imread("..\\Test\\Lena_24bits.bmp",IMREAD_COLOR);
    imshow("RGB",img);
    imshow("YCC->RGB", YCCtoRGB(RGBtoYCC(img)));
    waitKey(0);
    return 0;
}
