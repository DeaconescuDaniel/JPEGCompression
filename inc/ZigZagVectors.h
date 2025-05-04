#ifndef JPEG_COMPRESSOR_ZIGZAGVECTORS_H
#define JPEG_COMPRESSOR_ZIGZAGVECTORS_H

#include <opencv2/opencv.hpp>
#include "ImageBlock.h"

using namespace cv;
using namespace std;

const int zigZagScanIndex[64] = {
        0,  1,  5,  6, 14, 15, 27, 28,
        2,  4,  7, 13, 16, 26, 29, 42,
        3,  8, 12, 17, 25, 30, 41, 43,
        9, 11, 18, 24, 31, 40, 44, 53,
        10, 19, 23, 32, 39, 45, 52, 54,
        20, 22, 33, 38, 46, 51, 55, 60,
        21, 34, 37, 47, 50, 56, 59, 61,
        35, 36, 48, 49, 57, 58, 62, 63
};

struct ZigZagVectors {
    int8_t dcY;
    int8_t dcCb;
    int8_t dcCr;

    vector<pair<unsigned int, char>> acY;
    vector<pair<unsigned int, char>> acCb;
    vector<pair<unsigned int, char>> acCr;

    explicit ZigZagVectors(const ImageBlock &block);
    static vector<pair<unsigned int, char>> encodeAC(const vector<char>& acInput);
};



#endif //JPEG_COMPRESSOR_ZIGZAGVECTORS_H
