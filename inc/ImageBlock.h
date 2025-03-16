#ifndef JPEG_COMPRESSOR_IMAGEBLOCK_H
#define JPEG_COMPRESSOR_IMAGEBLOCK_H

#include <opencv2/opencv.hpp>

using namespace cv;

struct ImageBlock {
    ImageBlock(const Mat &luma, const Mat &cb, const Mat &cr);

    Mat Y;
    Mat Cb;
    Mat Cr;

    explicit ImageBlock(int subsampleRatio = 2, int type = CV_8UC1);

    ImageBlock& operator=(const ImageBlock& other);

};

#endif //JPEG_COMPRESSOR_IMAGEBLOCK_H
