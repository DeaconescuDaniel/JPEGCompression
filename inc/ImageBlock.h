//
// Created by bests on 13/03/2025.
//

#ifndef JPEG_COMPRESSOR_IMAGEBLOCK_H
#define JPEG_COMPRESSOR_IMAGEBLOCK_H


struct ImageBlock {
    unsigned char data[8][8];

    ImageBlock();

    ImageBlock &operator=(const ImageBlock &other);
};


#endif //JPEG_COMPRESSOR_IMAGEBLOCK_H
