cmake_minimum_required(VERSION 3.28)
project(JPEG_Compressor)

# Find OpenCV
find_package(OpenCV REQUIRED)
add_subdirectory(googletest)
include_directories(${OpenCV_INCLUDE_DIRS} inc)
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)

set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
        src/rgbToYCC.cpp
        src/dct.cpp
        src/ImageBlockMatrix.cpp
        src/ImageBlock.cpp
        src/quantization.cpp
        src/ZigZagVectors.cpp
)

set(TESTS
        test/allTests.cpp
        src/rgbToYCC.cpp
        src/dct.cpp
        src/ImageBlockMatrix.cpp
        src/ImageBlock.cpp
        src/quantization.cpp
        src/ZigZagVectors.cpp
        test/testChannelConversion.cpp
        test/testImageBlocks.cpp
        test/testDCT.cpp
        test/testQuantization.cpp
        test/testEncoding.cpp
        test/testHelpers.cpp
        test/inputs.h
        test/outputs.h
)

add_executable(JPEG_Compressor main.cpp ${SOURCES})
add_executable(ALL_TESTS ${TESTS})

target_link_libraries(ALL_TESTS PRIVATE gmock_main gtest_main ${OpenCV_LIBS})

# Link OpenCV
target_link_libraries(JPEG_Compressor ${OpenCV_LIBS})
