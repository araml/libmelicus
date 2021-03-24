#!/bin/bash
mkdir -p build
cd build
cmake -DBUILD_TESTS=ON -G Ninja .. # -DMPD_SUPPORT=ON ..
ninja

