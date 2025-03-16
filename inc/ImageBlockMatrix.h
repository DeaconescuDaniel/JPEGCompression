#ifndef JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H
#define JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H

#include <vector>
#include "ImageBlock.h"

using namespace cv;

struct ImageBlockMatrix {

    ImageBlockMatrix(int rows,int cols, int subsampleRatio=2);
    explicit ImageBlockMatrix(const cv::Mat& img, int subsampleRatio=2);
    [[nodiscard]] ImageBlock getBlockAt(int rowIndex,int colIndex) const;
    void setBlockAt(int rowIndex, int colIndex, const ImageBlock& block);

private:
    int m_rows,m_cols;
    std::vector<ImageBlock> m_blockMatrix;
    int m_subsampleRatio;
public:
    [[nodiscard]] int getCols() const;

    [[nodiscard]] int getRows() const;

    Mat reconstructImage(ImageBlockMatrix blockMatrix) const;
};


#endif //JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H
