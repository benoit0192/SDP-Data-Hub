#!/bin/bash

mkdir -p build
cd build
cmake                        \
    -DENABLE_TESTING=OFF     \
    -DCMAKE_BUILD_TYPE=Debug \
    ..
cmake --build .
#cmake --build . --target install
#cmake --build . -DENABLE_TESTING=OFF --target install
