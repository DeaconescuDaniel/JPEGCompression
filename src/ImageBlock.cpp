#include "ImageBlock.h"

ImageBlock::ImageBlock(int subsampleRatio, int type)
{
    Y = Mat(8, 8, type, Scalar(0));

    const int chromaSize = 8 / subsampleRatio;
    Cb = Mat(chromaSize, chromaSize, type, Scalar(0));
    Cr = Mat(chromaSize, chromaSize, type, Scalar(0));
}

ImageBlock::ImageBlock(const cv::Mat& luma, const cv::Mat& cb, const cv::Mat& cr) {
    Y = luma.clone();
    Cb = cb.clone();
    Cr = cr.clone();
}

ImageBlock& ImageBlock::operator=(const ImageBlock& other) {
    if (this != &other) {
        Y = other.Y.clone();
        Cb = other.Cb.clone();
        Cr = other.Cr.clone();
    }
    return *this;
}
