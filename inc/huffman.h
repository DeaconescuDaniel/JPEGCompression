#ifndef JPEG_COMPRESSOR_HUFFMAN_H
#define JPEG_COMPRESSOR_HUFFMAN_H

#include <map>
#include <vector>
#include <queue>
#include <utility>
#include <string>


std::string encodeStandardDCDifference(int diff, bool isC);
std::string encodeStandardAC(unsigned int run, int size, bool isC, int value);


#endif //JPEG_COMPRESSOR_HUFFMAN_H
