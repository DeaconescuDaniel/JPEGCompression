#include "ImageBlockMatrix.h"


ImageBlockMatrix::ImageBlockMatrix(const int rows, const int cols)
{
    blockMatrix.resize(rows*cols);
}

void ImageBlockMatrix::setBlockAt(const int rowIndex, int colIndex, ImageBlock block)
{
    blockMatrix[rowIndex * cols + colIndex] = block;
}

ImageBlock ImageBlockMatrix::getBlockAt(const int rowIndex, int colIndex) const
{
    return blockMatrix[rowIndex * cols + colIndex];
}
