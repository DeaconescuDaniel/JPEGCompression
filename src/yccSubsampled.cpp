#include "yccSubsampled.h"

yccSubsampled::yccSubsampled(cv::Mat yccImg, const int subsampleRatio)
{
    std::vector<Mat> channels(3);
    split(yccImg, channels);
    Y = channels[0];        // Pastram Luminance
    Mat Cb = channels[1], Cr = channels[2];

    int newCols = Cb.cols / subsampleRatio;
    int newRows = Cb.rows / subsampleRatio;

    // Facem subsampling biliniar
    resize(Cb, CbSubsampled, Size(newCols, newRows), 0, 0, INTER_LINEAR);
    resize(Cr, CrSubsampled, Size(newCols, newRows), 0, 0, INTER_LINEAR);
}