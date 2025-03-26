#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "rgbToYCC.h"
#include "ImageBlockMatrix.h"
#include "dct.h"

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
            //quantizeBlock(transformedBlock);
            blockMatrix.setBlockAt(i,j,transformedBlock);
        }
    }

}

int main() {
    // Load image and convert to YCC
    Mat img = imread(R"(..\test\images\Lena_24bits.bmp)", IMREAD_COLOR);
    if (img.empty()) {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    return 0;
}
