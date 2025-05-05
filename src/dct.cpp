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


Mat applyDCT(Mat& block) {
    block.convertTo(block, CV_32F);

    block -= 128.0f;


    dct(block, block);

    return block;
}

void quantizeBlock(Mat& block) {
    block  /= quantTableYMat;
    block  = block + 0.5f;
    block.convertTo(block, CV_8S);
}
