cmake_minimum_required(VERSION 3.0)
project(MerkleTree)

option(BLOOM "Use bloom filter in tree node" ON)

set(CMAKE_CXX_FLAGS "-Wall -g")

configure_file(
  "${PROJECT_SOURCE_DIR}/include/config.h.in"
  "${PROJECT_SOURCE_DIR}/include/config.h"
)

include_directories(include)

add_executable(benchmark tests/benchmark.cc)
target_link_libraries(benchmark crypto++)

add_executable(bloom_filter_test tests/bloom_filter_test.cc)
target_link_libraries(bloom_filter_test crypto++)

add_executable(build_merkletree tests/build_merkletree.cc)
target_link_libraries(build_merkletree crypto++)