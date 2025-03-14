#ifndef JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H
#define JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H

#include <vector>
#include "ImageBlock.h"

using namespace cv;

struct ImageBlockMatrix {

    ImageBlockMatrix(int rows,int cols, int subsampleRatio=2);
    ImageBlockMatrix(cv::Mat img, int subsampleRatio=2);
    [[nodiscard]] ImageBlock getBlockAt(int rowIndex,int colIndex) const;
    void setBlockAt(int rowIndex, int colIndex, const ImageBlock& block);

private:
    int m_rows,m_cols;
    std::vector<ImageBlock> m_blockMatrix;
public:
    int getCols() const;

    int getRows() const;

    cv::Mat reconstructImage(ImageBlockMatrix blockMatrix);
};


#endif //JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H
