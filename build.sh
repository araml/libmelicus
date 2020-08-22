#!/bin/bash
rm -rf build
mkdir build
cd build
#cmake ..
cmake -G Ninja -DMPD_SUPPORT=ON ..
#cmake --build . -- -j16
ninja

