#ifndef JPEG_COMPRESSOR_DCT_H
#define JPEG_COMPRESSOR_DCT_H

#include "ImageBlock.h"

ImageBlock discreteCosineTransform(ImageBlock block);

ImageBlock applyDCT(ImageBlock& block);

void quantizeBlock(ImageBlock& block);

#endif //JPEG_COMPRESSOR_DCT_H
