#ifndef JPEG_COMPRESSOR_DCT_H
#define JPEG_COMPRESSOR_DCT_H

#include "ImageBlock.h"

ImageBlock discreteCosineTransform(ImageBlock block);

void discreteCosineTransform(Mat& mat);

#endif //JPEG_COMPRESSOR_DCT_H
