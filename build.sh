#!/bin/bash
rm -rf build
mkdir build
cd build
#cmake ..
cmake -DBUILD_TESTS=ON -G Ninja .. # -DMPD_SUPPORT=ON ..
#cmake --build . -- -j16
ninja

