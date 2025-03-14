#include "quantization.h"
#include "ImageBlock.h"

void quantizeBlock(ImageBlock& block)
{
    block.Y /= quantTableYMat;
    block.Cb /= quantTableCMat;
    block.Cr /= quantTableCMat;
}