#include <iostream>
#include <chrono>
#include <cassert>
#include <sstream>
#include <iomanip>
#include "../include/merkletree.h"
#include "random.h"

#define NUM_OF_DATA     10000
#define BLOOM_MULTIPLE  0.1

const size_t BLOOM_SIZE = NUM_OF_DATA * BLOOM_MULTIPLE;

using merkletree::MerkleTree;

std::string human_readable(double size) {
  static const std::string suffix[] = {
    "B",
    "KB",
    "MB",
    "GB"
  };
  const int arr_len = 4;

  std::ostringstream out;
  out.precision(2);
  for (int divs = 0; divs < arr_len; ++divs) {
    if (size >= 1024.0) {
      size /= 1024.0;
    } else {
      out << std::fixed << size;
      return out.str() + suffix[divs];
    }
  }
  out << std::fixed << size;
  return out.str() + suffix[arr_len - 1];
}

int main(int argc, char** argv) {
#ifdef BLOOM
  std::cout << "Use Bloom Filter" << std::endl;
#else
  std::cout << "Not Use Bloom Filter" << std::endl;
#endif

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " false-data-percent" << std::endl;
    exit(-1);
  }

  double false_percent = atof(argv[1])/100.0;
  size_t TEST_SIZE = false_percent >= 1.0 ? (NUM_OF_DATA) : ((double)NUM_OF_DATA / (1.0 - false_percent));

  std::cout << "Num of Data:    " << NUM_OF_DATA << std::endl;
  std::cout << "Bloom Multiple: " << BLOOM_MULTIPLE << std::endl;
  std::cout << "False Percent:  " << false_percent << std::endl;
  std::cout << "Test Size:      " << TEST_SIZE << std::endl;

  std::vector<std::string*> data;
  for (size_t i = 0; i < NUM_OF_DATA; ++i)
    data.emplace_back(new std::string(std::to_string(i)));

  auto start_time = std::chrono::high_resolution_clock::now();
  MerkleTree<std::string, BLOOM_SIZE> tree(data);
  auto stop_time = std::chrono::high_resolution_clock::now();

  std::cout << "Build Time:     " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time-start_time).count() << "ms" << std::endl;
  std::cout << "Data Count:     " << tree.LeafSize() << std::endl;
  std::cout << "Tree Depth:     " << tree.Level() << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Memory Use:     " << human_readable(tree.ByteUse()) << std::endl;
#ifdef BLOOM
  std::cout << "Fill Rate:      " << tree.BloomFillRate() * 100 << "%" << std::endl;
#endif

  tree.Verify();

  for (auto d : data)
    assert(tree.Exist(d));

  std::vector<std::string*> test_data;
  std::vector<size_t> test_int;
  std::vector<bool> ans;
  if (false_percent >= 1.0) {
    for (size_t i = 0; i < TEST_SIZE; ++i)
      test_int.emplace_back(i+NUM_OF_DATA);
  } else {
    for (size_t i = 0; i < TEST_SIZE; ++i)
      test_int.emplace_back(i);
  }

  Random rand(0, TEST_SIZE-1);
  for (size_t i = 0; i < TEST_SIZE; ++i)
    std::swap(test_int[i], test_int[rand.Next()]);
  for (size_t i = 0; i < TEST_SIZE; ++i) {
    ans.push_back(test_int[i] < NUM_OF_DATA);
    test_data.push_back(new std::string(std::to_string(test_int[i])));
  }

  start_time = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < TEST_SIZE; ++i)
    assert(tree.Exist(test_data[i]) == ans[i]);
  stop_time = std::chrono::high_resolution_clock::now();

#ifdef BLOOM
  std::cout << "Use Bloom:      " << (double)std::chrono::duration_cast<std::chrono::milliseconds>(stop_time-start_time).count() / (double)TEST_SIZE * 1000.0 << "ms" << std::endl;
#else
  std::cout << "Not Use Bloom:  " << (double)std::chrono::duration_cast<std::chrono::milliseconds>(stop_time-start_time).count() / (double)TEST_SIZE * 1000.0 << "ms" << std::endl;
#endif
}