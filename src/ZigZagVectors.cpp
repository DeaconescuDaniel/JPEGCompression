#include "ZigZagVectors.h"
#include "ImageBlock.h"

#include <utility>

ZigZagVectors::ZigZagVectors(vector<float> vectorY, vector<float> vectorCb, vector<float> vectorCr) :
    vectorY(std::move(vectorY)), vectorCb(std::move(vectorCb)), vectorCr(std::move(vectorCr)) {};


RunLengthZigZagVectors::RunLengthZigZagVectors(const ZigZagVectors& inVectors) {
    vectorY = encodeVector(inVectors.vectorY);
    vectorCb = encodeVector(inVectors.vectorCb);
    vectorCr = encodeVector(inVectors.vectorCr);
}

vector<pair<unsigned int, float>> RunLengthZigZagVectors::encodeVector(const vector<float>& inputVector) {
    assert(!inputVector.empty());

    float currentKey = inputVector[0];
    unsigned int currentLength = 1;
    vector<pair<unsigned int, float>> output;

    for (size_t i = 1; i < inputVector.size(); i++) {
        if (inputVector[i] != currentKey) {
            output.emplace_back(currentLength, currentKey);
            currentKey = inputVector[i];
            currentLength = 1;
        } else {
            currentLength++;
        }
    }

    output.emplace_back(currentLength, currentKey);
    return output;
}

ZigZagVectors zigzagScan(const ImageBlock &block) {
    vector<float> vectorY(64);
    vector<float> vectorCb(64);
    vector<float> vectorCr(64);

    for (int i = 0; i < 64; i++) {
        int row = zigZagScanIndex[i] / 8;
        int col = zigZagScanIndex[i] % 8;
        vectorY[i] = block.Y.at<float>(row, col);
        vectorY[i] = block.Cb.at<float>(row, col);
        vectorY[i] = block.Cr.at<float>(row, col);
    }
    return ZigZagVectors(vectorY,vectorCb,vectorCr);
}

RunLengthZigZagVectors::RunLengthZigZagVectors() : vectorY(), vectorCb(), vectorCr() {}

