#include <iostream>
#include <chrono>
#include <sstream>
#include "../include/merkletree.h"

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
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " num-of-data" << std::endl;
    exit(-1);
  }

  size_t num_of_data = atoi(argv[1]);

  std::vector<std::string*> data;
  for (size_t i = 0; i < num_of_data; ++i)
    data.emplace_back(new std::string(std::to_string(i)));

  auto start_time = std::chrono::high_resolution_clock::now();
  MerkleTree<> tree(data);
  auto stop_time = std::chrono::high_resolution_clock::now();
  std::cout << "Build Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time-start_time).count() << "ms" << std::endl;
  std::cout << "Data Count: " << tree.LeafSize() << std::endl;
  std::cout << "Tree Depth: " << tree.Level() << std::endl;
  std::cout << "Memory Use: " << human_readable(tree.ByteUse()) << std::endl;
}