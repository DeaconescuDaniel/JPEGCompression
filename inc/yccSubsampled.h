#ifndef JPEG_COMPRESSOR_YCCSUBSAMPLED_H
#define JPEG_COMPRESSOR_YCCSUBSAMPLED_H

#include <opencv2/opencv.hpp>

using namespace cv;

struct yccSubsampled
{

    Mat Y;
    Mat CbSubsampled;
    Mat CrSubsampled;

    yccSubsampled(Mat yccImg, const int subsampleRatio);
};


#endif //JPEG_COMPRESSOR_YCCSUBSAMPLED_H
