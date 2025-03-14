#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "rgbToYCC.h"
#include "ImageBlockMatrix.h"
#include "dct.h"
#include "quantization.h"

using namespace cv;
using namespace std;

void compressJPG(Mat image)
{
    assert(image.channels() == 3);
    Mat yccImage = RGBtoYCC(image);
    ImageBlockMatrix blockMatrix = ImageBlockMatrix(yccImage);
    for(int i=0;i<blockMatrix.getRows();i++)
    {
        for(int j=0;j<blockMatrix.getCols();j++)
        {
            ImageBlock block = blockMatrix.getBlockAt(i,j);
            ImageBlock transformedBlock = discreteCosineTransform(block);
            quantizeBlock(transformedBlock);
            blockMatrix.setBlockAt(i,j,transformedBlock);
        }
    }

}

int main() {
    // Load image and convert to YCC
    Mat img = imread("..\\Test\\Lena_24bits.bmp", IMREAD_COLOR);
    if (img.empty()) {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    Mat yccImg = RGBtoYCC(img);
    vector<Mat> channels;
    split(yccImg, channels);
    Mat CbOrig = channels[1];

    // Open output file to store block data
    ofstream outFile("..\\data\\output.txt");

    // Write the entire original Y channel first
    outFile << "Original Cb Channel:\n";
    for (int i = 0; i < CbOrig.rows; i++) {
        for (int j = 0; j < CbOrig.cols; j++) {
            outFile << (int)CbOrig.at<uchar>(i, j) << " ";
        }
        outFile << "\n";
    }
    outFile << "\n";

    // Perform block splitting
    ImageBlockMatrix blocks = ImageBlockMatrix(yccImg);

    // Write block data (including Cb blocks)
    for (int i = 0; i < blocks.getRows(); i++) {
        for (int j = 0; j < blocks.getCols(); j++) {
            ImageBlock block = blocks.getBlockAt(i, j);

            // Write Cb blocks
            outFile << "Block (" << i << ", " << j << ") Cb:\n";
            for (int r = 0; r < block.Cb.rows; r++) {
                for (int c = 0; c < block.Cb.cols; c++) {
                    outFile << (int)block.Cb.at<uchar>(r, c) << " ";
                }
                outFile << "\n";
            }
            outFile << "\n";
        }
    }

    outFile.close();
    cout << "Original and block-split Cb channel saved to output.txt and images." << endl;

    return 0;
}
