//
// Created by bests on 13/03/2025.
//

#ifndef JPEG_COMPRESSOR_IMAGEBLOCK_H
#define JPEG_COMPRESSOR_IMAGEBLOCK_H

#include <opencv2/opencv.hpp>

using namespace cv;

struct ImageBlock {
    Mat Y;
    Mat Cb;
    Mat Cr;


    ImageBlock(int subsampleRatio);

    ImageBlock& operator=(const ImageBlock& other);
};

#endif //JPEG_COMPRESSOR_IMAGEBLOCK_H
