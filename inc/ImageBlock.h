#ifndef JPEG_COMPRESSOR_IMAGEBLOCK_H
#define JPEG_COMPRESSOR_IMAGEBLOCK_H

#include <opencv2/opencv.hpp>

using namespace cv;

struct ImageBlock {
    Mat Y;
    Mat Cb;
    Mat Cr;

    ImageBlock(int subsampleRatio = 2);

    ImageBlock& operator=(const ImageBlock& other);

public:
    int getSubsampleRatio() const;
};

#endif //JPEG_COMPRESSOR_IMAGEBLOCK_H
