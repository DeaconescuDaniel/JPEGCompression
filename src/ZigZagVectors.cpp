#include "ZigZagVectors.h"

#include <utility>
#include <cassert>

ZigZagVectors::ZigZagVectors(const Mat &block) {
    vector<char> tempY(64);

    for (int i = 0; i < 64; i++) {
        int row = zigZagScanIndex[i] / 8;
        int col = zigZagScanIndex[i] % 8;
        tempY[i] = block.at<char>(row, col);
    }

    // First element is DC
    dcY = tempY[0];

    // Rest are AC
    vector<char> acYInput(tempY.begin() + 1, tempY.end());

    acY = encodeAC(acYInput);
}

vector<pair<pair<unsigned char, unsigned char>, short>> ZigZagVectors::encodeAC(const vector<char> &inputVector) {
    assert(inputVector.size() == 63);  // AC coefficients: should be 63 after the DC value

    vector<pair<pair<unsigned char, unsigned char>, short>> output;
    int zeroRun = 0;

    for (size_t i = 0; i < inputVector.size(); ++i) {
        short val = inputVector[i];

        if (val == 0) {
            zeroRun++;
        } else {
            // Emit ZRL (15, 0) as needed
            while (zeroRun >= 16) {
                output.emplace_back(make_pair(15, 0), 0);  // JPEG ZRL
                zeroRun -= 16;
            }

            // SIZE = number of bits needed to represent the value
            unsigned char size = (val == std::numeric_limits<short>::min()) ? 15 : (unsigned char)(log2(abs(val)) + 1);

            // Emit (RUNLENGTH, SIZE) : VALUE
            output.emplace_back(make_pair(zeroRun, size), val);
            zeroRun = 0;
        }
    }

    // If trailing zeros, emit EOB
    if (zeroRun > 0) {
        output.emplace_back(make_pair(0, 0), 0);  // EOB
    }

    return output;
}