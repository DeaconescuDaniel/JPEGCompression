#ifndef JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H
#define JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H

#include <vector>
#include "ImageBlock.h"

struct ImageBlockMatrix {


    ImageBlockMatrix(const int rows,const int cols);
    ImageBlock getBlockAt(const int rowIndex,int colIndex) const;
    void setBlockAt(const int rowIndex, int colIndex, ImageBlock block);
private:
    int rows,cols;
    std::vector<ImageBlock> blockMatrix;
};


#endif //JPEG_COMPRESSOR_IMAGEBLOCKMATRIX_H
