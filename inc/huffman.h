#ifndef JPEG_COMPRESSOR_HUFFMAN_H
#define JPEG_COMPRESSOR_HUFFMAN_H

#include <map>
#include <vector>
#include <queue>
#include <utility>
#include <string>


std::string encodeStandardDCDifference(int diff);
std::string encodeStandardAC(int run, int size, int value);


#endif //JPEG_COMPRESSOR_HUFFMAN_H
