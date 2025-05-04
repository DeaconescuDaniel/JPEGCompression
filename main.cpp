#include <iostream>
#include <opencv2/opencv.hpp>
#include <bitset>
#include <fstream>
#include "rgbToYCC.h"
#include "ImageBlockMatrix.h"
#include "dct.h"
#include "ZigZagVectors.h"
#include "huffman.h"

using namespace cv;

std::vector<uint8_t> packBits(const std::string& bitStream) {
    std::vector<uint8_t> packed;
    uint8_t byte = 0;
    int bitCount = 0;

    for (char bit : bitStream) {
        byte = (byte << 1) | (bit - '0');
        bitCount++;

        if (bitCount == 8) {
            packed.push_back(byte);
            byte = 0;
            bitCount = 0;
        }
    }

    // Handle remaining bits by padding with zeros
    if (bitCount > 0) {
        byte = byte << (8 - bitCount);
        packed.push_back(byte);
    }

    return packed;
}

void writeJPEGFile(const std::string& filename,
                   const std::string& bitstreamY,
                   const std::string& bitstreamC,
                   int height,int width) {

    std::ofstream out(filename, std::ios::binary);
    auto writeMarker = [&](uint8_t marker1, uint8_t marker2) {
        out.put(marker1);
        out.put(marker2);
    };

    auto write16 = [&](uint16_t value) {
        out.put((value >> 8) & 0xFF);
        out.put(value & 0xFF);
    };

    auto writeSegment = [&](uint8_t marker, const std::vector<uint8_t>& data) {
        writeMarker(0xFF, marker);
        write16(data.size() + 2);
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
    };

    writeMarker(0xFF, 0xD8); // SOI

    std::vector<uint8_t> app0 = {
            'J', 'F', 'I', 'F', 0x00,
            0x01, 0x01, // version
            0x00,       // no density
            0x00, 0x01, 0x00, 0x01,
            0x00, 0x00  // no thumbnail
    };
    writeSegment(0xE0, app0);

    auto writeQuantTable = [&](const float* quant, uint8_t tableID) {
        std::vector<uint8_t> data;
        data.push_back(tableID); // Precision

        for (int i = 0; i < 64; ++i) {
            float val = quant[i];
            uint8_t qVal = static_cast<uint8_t>(std::clamp(std::round(val), 1.0f, 255.0f)); // Clamp to [1, 255]
            data.push_back(qVal);
        }

        writeSegment(0xDB, data); // DQT
    };
    extern float quantTableY[64];
    extern float quantTableC[64];
    writeQuantTable(quantTableY, 0);
    writeQuantTable(quantTableC, 1);

    std::vector<uint8_t> sof = {
            0x08,       // precision
            static_cast<uint8_t>(height >> 8), static_cast<uint8_t>(height & 0xFF), // height
            static_cast<uint8_t>(width >> 8),  static_cast<uint8_t>(width & 0xFF),  // width
            0x03,       // 3 components
            0x01, 0x11, 0x00, // Y, H=1 V=1, QTable ID = 0
            0x02, 0x11, 0x01, // Cb, use QTable 1
            0x03, 0x11, 0x01  // Cr, use QTable 1
    };
    writeSegment(0xC0, sof);

    auto writeStandardHuffmanTables = [&writeSegment]{
        // DC Luminance (Table 0)
        writeSegment(0xC4,{
                // DC luminance table (Table 0, Class 0)
                0x00,         // Table class and identifier (0x00 for DC luminance)

                // Code lengths
                0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01,
                0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                // Huffman values
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08,  0x09, 0x0A, 0x0B
        });

        // AC Luminance (Table 0)
        writeSegment(0xC4, {
                // AC luminance table (Table 0, Class 1)
                0x10,         // Table class and identifier (0x10 for AC luminance)

                // Code lengths
                0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03,
                0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D,

                // Huffman values
                0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
                0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
                0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
                0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
                0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
                0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
                0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
                0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
                0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
                0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
                0xF9, 0xFA
        });

        // DC Chrominance (Table 1)
        writeSegment(0xC4,{
                0x01,         // Table class and identifier (0x01 for DC chrominance)

                // Code lengths
                0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

                // Huffman values
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B
        });

        // AC Chrominance (Table 1)
        writeSegment(0xC4,{
                0x11,         // Table class and identifier (0x11 for AC chrominance)

                // Code lengths
                0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04,
                0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,

                // Huffman values
                0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
                0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
                0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
                0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
                0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
                0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
                0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
                0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
                0xF9, 0xFA
        });
    };


    writeStandardHuffmanTables();


    // --- Start of Scan (SOS) ---
    std::vector<uint8_t> sos = {
            0x03, // number of components
            0x01, 0x00, // Y uses DC0/AC0
            0x02, 0x11, // Cb uses DC1/AC1
            0x03, 0x11, // Cr uses DC1/AC1
            0x00, 0x3F, 0x00 // spectral selection
    };
    writeSegment(0xDA, sos);

    // --- Compressed image data ---
    auto writeBitstream = [&](const std::string& bits) {
        std::vector<uint8_t> packed = packBits(bits);
        for (uint8_t byte : packed) {
            out.put(byte);
            if (byte == 0xFF) out.put(0x00); // byte stuffing
        }
    };

    writeBitstream(bitstreamY);
    writeBitstream(bitstreamC);

    // --- End of Image ---
    writeMarker(0xFF, 0xD9); // EOI

    out.close();
    std::cout << "JPEG file written: " << filename << std::endl;
}


void compressJPG(Mat image) {
    assert(image.channels() == 3);

    Mat yccImage = RGBtoYCC(image);
    ImageBlockMatrix blockMatrix(yccImage);

    // Step 1: Apply DCT and Quantization
    for (int i = 0; i < blockMatrix.getRows(); i++) {
        for (int j = 0; j < blockMatrix.getCols(); j++) {
            ImageBlock block = blockMatrix.getBlockAt(i, j);
            ImageBlock transformedBlock = applyDCT(block);
            quantizeBlock(transformedBlock);
            blockMatrix.setBlockAt(i, j, transformedBlock);
        }
    }

    std::vector<int8_t> dcYList, dcCList;
    std::vector<std::vector<std::pair<unsigned int, char>>> acYList, acCList;

    for (int i = 0; i < blockMatrix.getRows(); i++) {
        for (int j = 0; j < blockMatrix.getCols(); j++) {
            ImageBlock block = blockMatrix.getBlockAt(i, j);
            ZigZagVectors encoded(block);

            dcYList.push_back(encoded.dcY);
            dcCList.push_back(encoded.dcCb);
            dcCList.push_back(encoded.dcCr);

            acYList.push_back(encoded.acY);
            acCList.push_back(encoded.acCb);
            acCList.push_back(encoded.acCr);
        }
    }

    std::string bitstreamY, bitstreamC;
    int prevDCY = 0, prevDCCb = 0, prevDCCr = 0;

    for (int i = 0; i < dcYList.size(); ++i) {
        int diffY = dcYList[i] - prevDCY;
        bitstreamY += encodeStandardDCDifference(diffY, false);  // Luminance
        prevDCY = dcYList[i];
    }

    for (int i = 0; i < dcCList.size(); i += 2) {
        int diffCb = dcCList[i] - prevDCCb;
        int diffCr = dcCList[i + 1] - prevDCCr;

        bitstreamC += encodeStandardDCDifference(diffCb, true);  // Chrominance Cb
        bitstreamC += encodeStandardDCDifference(diffCr, true);  // Chrominance Cr

        prevDCCb = dcCList[i];
        prevDCCr = dcCList[i + 1];
    }

    for (int i = 0; i < acYList.size(); ++i) {
        const auto& blockAC = acYList[i];

        for (const auto& [run, value] : blockAC) {
            int size = (value == 0) ? 0 : static_cast<int>(std::log2(std::abs(value))) + 1;
            bitstreamY += encodeStandardAC(run, size, false, value);  // Luminance AC
        }
    }

    for (int i = 0; i < acCList.size(); ++i) {
        const auto& blockAC = acCList[i];

        for (const auto& [run, value] : blockAC) {
            int size = (value == 0) ? 0 : static_cast<int>(std::log2(std::abs(value))) + 1;
            bitstreamC += encodeStandardAC(run, size, true, value);  // Chrominance AC
        }
    }

    auto printBitstream = [](const std::string& stream, const std::string& label) {
        std::vector<uint8_t> packed = packBits(stream);
        std::cout << label << ": ";
        for (uint8_t byte : packed) printf("%02X ", byte);
        std::cout << std::endl;
    };

    printBitstream(bitstreamY,  "Y");
    printBitstream(bitstreamC, "C");

    std::cout << "Total Bitstream Length: "
              << bitstreamY.size() + bitstreamC.size()
              << " bits\n";

    writeJPEGFile("output.jpg", bitstreamY, bitstreamC,image.rows,image.cols);
}


int main() {
    // Load image and convert to YCC
    Mat img = imread(R"(..\test\images\flowers_24bits.bmp)", IMREAD_COLOR);
    if (img.empty()) {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    compressJPG(img);

    return 0;
}
