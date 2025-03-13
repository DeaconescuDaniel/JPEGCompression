cmake_minimum_required(VERSION 3.28)
project(JPEG_Compressor)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} inc)

set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
        src/rgbToYCC.cpp
        src/subsampling.cpp
        src/dct.cpp
        src/ImageBlockMatrix.cpp
        src/ImageBlock.cpp
        src/yccSubsampled.cpp
        src/quantization.cpp
)

add_executable(JPEG_Compressor main.cpp ${SOURCES})

# Link OpenCV
target_link_libraries(JPEG_Compressor ${OpenCV_LIBS})
