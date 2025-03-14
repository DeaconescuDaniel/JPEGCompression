#include "dct.h"

ImageBlock discreteCosineTransform(ImageBlock block)
{
    Mat luma  = block.Y;
    Mat Cb = block.Cb;
    Mat Cr = block.Cr;

    cv::resize(block.Cb, block.Cb, cv::Size(8, 8), 0, 0, cv::INTER_LINEAR);
    cv::resize(block.Cr, block.Cr, cv::Size(8, 8), 0, 0, cv::INTER_LINEAR);

    discreteCosineTransform(block.Y);
    discreteCosineTransform(block.Cb);
    discreteCosineTransform(block.Cr);
}

void discreteCosineTransform(Mat &block) {
    block.convertTo(block, CV_32F);
    block -= 128.0f;
    dct(block, block);
}

