#include "dct.h"

ImageBlock applyDCT(ImageBlock& block) {
    block.Y.convertTo(block.Y, CV_32F);
    block.Cb.convertTo(block.Cb, CV_32F);
    block.Cr.convertTo(block.Cr, CV_32F);

    block.Y -= 128.0f;
    block.Cb -= 128.0f;
    block.Cr -= 128.0f;

    Mat resizedCb, resizedCr;
    resize(block.Cb, resizedCb, Size(8, 8), 0, 0, INTER_LINEAR);
    resize(block.Cr, resizedCr, Size(8, 8), 0, 0, INTER_LINEAR);

    dct(block.Y, block.Y);
    dct(resizedCb, block.Cb);
    dct(resizedCr, block.Cr);

    return block;
}

