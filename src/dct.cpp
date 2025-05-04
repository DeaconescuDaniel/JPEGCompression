#include "dct.h"

float quantTableY[64] = {
        3,  2,  2,  2,  2,  2,  3,
        2,  2,  2,  3,  3,  3,  3,  4,
        6,  4,  4,  4,  4,  4,  8,  6,
        6,  5,  6,  9,  8, 10, 10,  9,
        8,  9,  9, 10, 12, 15, 12, 10,
        11, 14, 11,  9,  9, 13, 17, 13,
        14, 15, 16, 16, 17, 16, 10, 12,
        18, 19, 18, 16, 19, 15, 16, 16,16
};

//float quantTableY[64] = {
//        6, 4, 4, 6, 10, 16, 20, 24,
//        5, 5, 6, 8, 10, 23, 24, 22,
//        6, 5, 6, 10, 16, 23, 28, 22,
//        6, 7, 9, 12, 20, 35, 32, 25,
//        7, 9, 15, 22, 27, 44, 41, 31,
//        10, 14, 22, 26, 32, 42, 45, 37,
//        20, 26, 31, 35, 41, 48, 48, 40,
//        29, 37, 38, 39, 45, 40, 41, 40};
Mat quantTableYMat = Mat(8,8,CV_32F,quantTableY);


float quantTableC[64] = {
        3,  3,  3,  4,  3,  4,  8,  4,  4,  8, 16, 11,  9, 11, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};

//float quantTableC[64] = {
//        10, 8, 9, 9, 9, 8, 10, 9,
//        9, 9, 10, 10, 10, 11, 12, 17,
//        13, 12, 12, 12, 12, 20, 16, 16,
//        14, 17, 18, 20, 23, 23, 22, 20,
//        25, 25, 25, 25, 25, 25, 25, 25,
//        25, 25, 25, 25, 25, 25, 25, 25,
//        25, 25, 25, 25, 25, 25, 25, 25,
//        25, 25, 25, 25, 25, 25, 25, 25};
Mat quantTableCMat = Mat(8,8,CV_32F,quantTableC);


ImageBlock applyDCT(ImageBlock& block) {
    block.Y.convertTo(block.Y, CV_32F);
    block.Cb.convertTo(block.Cb, CV_32F);
    block.Cr.convertTo(block.Cr, CV_32F);

    block.Y -= 128.0f;

    Mat resizedCb, resizedCr;
    resize(block.Cb, resizedCb, Size(8, 8), 0, 0, INTER_LINEAR);
    resize(block.Cr, resizedCr, Size(8, 8), 0, 0, INTER_LINEAR);

    resizedCb -= 128.0f;
    resizedCr -= 128.0f;

    dct(block.Y, block.Y);
    dct(resizedCb, block.Cb);
    dct(resizedCr, block.Cr);

    return block;
}

void quantizeBlock(ImageBlock& block) {
    block.Y  /= quantTableYMat;
    block.Cb /= quantTableCMat;
    block.Cr /= quantTableCMat;

    block.Y  = block.Y + 0.5f;
    block.Cb = block.Cb + 0.5f;
    block.Cr = block.Cr + 0.5f;

    block.Y.convertTo(block.Y, CV_8S);
    block.Cb.convertTo(block.Cb, CV_8S);
    block.Cr.convertTo(block.Cr, CV_8S);
}
