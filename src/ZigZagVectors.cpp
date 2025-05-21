#include "ZigZagVectors.h"
#include "ImageBlock.h"

#include <utility>
#include <cassert>

ZigZagVectors::ZigZagVectors(const ImageBlock &block) {
    vector<char> tempY(64), tempCb(64), tempCr(64);

    for (int i = 0; i < 64; i++) {
        int row = zigZagScanIndex[i] / 8;
        int col = zigZagScanIndex[i] % 8;
        tempY[i] = block.Y.at<char>(row, col);
        tempCb[i] = block.Cb.at<char>(row, col);
        tempCr[i] = block.Cr.at<char>(row, col);
    }

    // First element is DC
    dcY = tempY[0];
    dcCb = tempCb[0];
    dcCr = tempCr[0];

    // Rest are AC
    vector<char> acYInput(tempY.begin() + 1, tempY.end());
    vector<char> acCbInput(tempCb.begin() + 1, tempCb.end());
    vector<char> acCrInput(tempCr.begin() + 1, tempCr.end());

    acY = encodeAC(acYInput);
    acCb = encodeAC(acCbInput);
    acCr = encodeAC(acCrInput);
}


vector<pair<unsigned int, char>> ZigZagVectors::encodeAC(const vector<char>& inputVector) {
    assert(!inputVector.empty());

    vector<pair<unsigned int, char>> output;
    int zeroRun = 0;

    for (size_t i = 0; i < inputVector.size(); ++i) {
        char val = inputVector[i];

        if (val == 0) {
            zeroRun++;
        } else {
            // Handle long runs of zeros by inserting multiple (15, 0)
            while (zeroRun >= 16) {
                output.emplace_back(15, 0);
                zeroRun -= 16;
            }

            // If there are remaining zeros, emit them with the current value
            if (zeroRun > 0) {
                output.emplace_back(zeroRun, val);
                zeroRun = 0;
            } else {
                // If no preceding zeros, the run length for the non-zero value should be zero
                output.emplace_back(0, val);
            }
        }
    }

    // If the last coefficients are all zero, emit an EOB
    if (!output.empty() && output.back().second != 0) {
        output.emplace_back(0, 0); // EOB
    } else if (output.empty()) {
        // All zeros, so emit only EOB
        output.emplace_back(0, 0);
    }

    return output;
}