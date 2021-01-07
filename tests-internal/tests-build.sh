#!/bin/bash

mkdir cmake-build-$1
cd cmake-build-$1
cmake .. -DCMAKE_BUILD_TYPE=$1
make
./$2
