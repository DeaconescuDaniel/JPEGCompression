#include "quantization.h"

void quantizeBlock(ImageBlock& block)
{
    block.Y /= quantTableYMat;
    block.Cb /= quantTableCMat;
    block.Cr /= quantTableCMat;
}