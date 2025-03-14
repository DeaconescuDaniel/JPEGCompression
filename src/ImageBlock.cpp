#include "ImageBlock.h"

ImageBlock::ImageBlock(int subsampleRatio)
{
    Y = Mat(8, 8, CV_8UC1, Scalar(0));

    const int chromaSize = 8 / subsampleRatio;
    Cb = Mat(chromaSize, chromaSize, CV_8UC1, Scalar(0));
    Cr = Mat(chromaSize, chromaSize, CV_8UC1, Scalar(0));
}


ImageBlock& ImageBlock::operator=(const ImageBlock& other) {
    if (this != &other) {
        Y = other.Y.clone();
        Cb = other.Cb.clone();
        Cr = other.Cr.clone();
    }
    return *this;
}
