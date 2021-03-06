cmake_minimum_required(VERSION 2.8)

project(BIGINT)
set(CMAKE_CXX_STANDARD 11)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(big_integer_testing
               test/big_integer_testing.cpp
               bigint/big_integer.h
               bigint/big_integer.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc 
               test/big_integer_gmp.cpp 
               test/big_integer_gmp.h)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined,address,leak -fno-sanitize-recover=all -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(big_integer_testing -lgmp -lpthread)
