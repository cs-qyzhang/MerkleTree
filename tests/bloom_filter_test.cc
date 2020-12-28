#include <iostream>
#include <chrono>
#include <cassert>
#include "../include/merkletree.h"
#include "../include/bloomfilter.h"

using merkletree::MerkleTree;
using merkletree::BloomFilter;

const size_t DATA_SIZE = 100000;

int main(int argc, char** argv) {
  std::vector<std::string*> data;
  for (size_t i = 0; i < DATA_SIZE; ++i)
    data.push_back(new std::string(std::to_string(i)));

  BloomFilter<std::string, DATA_SIZE*4> filter(data);
  for (auto d : data) {
    assert(filter.MaybeExist(d));
  }

  std::cout << filter.FillRate() << std::endl;
}