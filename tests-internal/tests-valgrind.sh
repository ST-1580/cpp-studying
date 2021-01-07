#!/bin/bash

cd cmake-build-Release
valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no --error-exitcode=1 ./$1
