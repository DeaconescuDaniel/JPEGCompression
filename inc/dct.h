#ifndef JPEG_COMPRESSOR_DCT_H
#define JPEG_COMPRESSOR_DCT_H

#include <opencv2/opencv.hpp>

using namespace cv;

Mat applyDCT(Mat& block);

void quantizeBlock(Mat& block);


#endif //JPEG_COMPRESSOR_DCT_H
