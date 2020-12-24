cmake_minimum_required(VERSION 3.0)
project(MerkleTree)

include_directories(include)

add_executable(basic_test tests/basic_test.cc)
target_link_libraries(basic_test crypto++)