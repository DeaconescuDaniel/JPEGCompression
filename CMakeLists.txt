cmake_minimum_required(VERSION 3.28)
project(Proiect)

# --- Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

# --- Define executable
add_executable(Proiect main.cpp)

# --- Link OpenCV libraries
target_link_libraries(Proiect ${OpenCV_LIBS})
