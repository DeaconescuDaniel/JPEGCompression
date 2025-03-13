#include "ImageBlock.h"
#include <cstring>

ImageBlock::ImageBlock()
{
    memset(data, 0, sizeof(data));
}

ImageBlock &ImageBlock::operator=(const ImageBlock &other)
{
    if (this != &other) {
        std::memcpy(this->data, other.data, sizeof(data));
    }
    return *this;
}
