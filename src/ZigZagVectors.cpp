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

vector<pair<unsigned int, char>> ZigZagVectors::encodeAC(const vector<char>& inputVector) {
    assert(!inputVector.empty());

    vector<pair<unsigned int, char>> output;

    if (inputVector.empty()) {
        return output;
    }

    char currentVal = inputVector[0];
    unsigned int run = 1;  // We've already seen the first element

    for (size_t i = 1; i < inputVector.size(); ++i) {
        if (inputVector[i] == currentVal && run < 15) {
            // If the current value is the same as the previous and run is < 15, continue counting the run
            run++;
        } else {
            if (currentVal != 0) {
                // If the current value is not zero, store the run
                output.emplace_back(run, currentVal);
            } else {
                // Handle runs of zeros (skip adding to the output and add EOB after a long sequence of zeros)
                if (run > 0) {
                    // If we encounter a sequence of zeros, we do not add the zero values directly.
                    // Instead, we'll eventually add an EOB marker after a certain run-length limit.
                    if (run >= 16) {
                        // If we encounter 16 zeros, add the EOB marker to signal the end of the block
                        output.emplace_back(0, 0);  // EOB marker, which can be handled in your Huffman encoding
                    }
                }
            }
            // Now move to the next element
            currentVal = inputVector[i];
            run = 1;
        }
    }

    // Handle the last run after finishing the loop
    if (currentVal != 0) {
        output.emplace_back(run, currentVal);
    } else if (run >= 16) {
        // If the final run is a large sequence of zeros, add EOB after 16 zeros
        output.emplace_back(0, 0);  // EOB marker
    }

    return output;
}