#include <iostream>
#include "merkletree.h"

using merkletree::MerkleTree;

const size_t DATA_SIZE = 1000;

int main(void) {
  std::vector<std::string*> data;
  for (int i = 0; i < DATA_SIZE; ++i)
    data.emplace_back(new std::string(std::to_string(i)));

  MerkleTree<std::string> tree(data);
  std::cout << "Leaf Size: " << tree.LeafSize() << std::endl;
  std::cout << "Level:     " << tree.Level() << std::endl;
  tree.Verify();
}