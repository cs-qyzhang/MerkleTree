#pragma once

#include <cstddef>
#include <utility>
#include <functional>
#include <memory>
#include <vector>
#include <deque>
#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>

namespace merkletree {

const int sha256_size = 32;

// default hash object
struct SHA256Hash {
  byte hash_val[sha256_size];

  SHA256Hash(const std::string& data) {
    CryptoPP::SHA256 hash;
    hash.Update((const byte*)data.data(), data.size());
    hash.Final(hash_val);
  }

  SHA256Hash(std::string* data) {
    CryptoPP::SHA256 hash;
    hash.Update((const byte*)data->data(), data->size());
    hash.Final(hash_val);
  }

  SHA256Hash(const SHA256Hash& a) {
    SHA256Hash(std::string((char*)a.hash_val, sha256_size));
  }

  SHA256Hash(const SHA256Hash& a, const SHA256Hash& b) {
    SHA256Hash(std::string((char*)a.hash_val, sha256_size) + std::string((char*)b.hash_val, sha256_size));
  }

  bool operator==(const SHA256Hash& a) const {
    return memcmp(hash_val, a.hash_val, sha256_size) == 0;
  }
};

static_assert(sizeof(SHA256Hash) == sha256_size);

template< typename T, typename Hash = SHA256Hash >
class MerkleTree {
 public:
  MerkleTree() : root_(nullptr), nr_leaf_(0), level_(0) {}
  MerkleTree(const std::vector<T*>& data) { BuildFromVector(data); }

  void BuildFromVector(const std::vector<T*>& data) {
    std::deque<Node*> nodes;
    for (auto d : data)
      nodes.emplace_back(new Node(d));
    nr_leaf_ = nodes.size();
    level_ = 1;
    while (nodes.size() != 1) {
      BuildUpperLevel_(nodes);
      level_++;
    }
    root_ = nodes.front();
  }

  bool Verify() const { return Verify_(root_); }
  size_t LeafSize() const { return nr_leaf_; }
  int Level() const { return level_; }

 private:
  struct Node {
    Node*  parent;
    Node*  left;    // nullptr if is leaf
    union {
      Node*  right;
      T*     data;
    };
    Hash hash;

    Node(T* d) : parent(nullptr), left(nullptr), data(d), hash(data) {}

    Node(Node* l) : parent(nullptr), left(l), right(nullptr), hash(l->hash) {
      l->parent = this;
    }

    Node(Node* l, Node* r) : parent(nullptr), left(l), right(r), hash(l->hash, r->hash) {
      l->parent = this;
      r->parent = this;
    }

    bool IsLeaf() const { return left == nullptr; }
    T* Data() const { return (T*)data; }

    bool Verify() const {
      if (IsLeaf())
        return hash == Hash(data);
      else
        return hash == Hash(left->hash, right->hash);
    }
  };

  Node*     root_;
  size_t    nr_leaf_;
  int       level_;

  size_t BuildUpperLevel_(std::deque<Node*>& nodes) {
    bool is_odd = nodes.size() % 2;
    size_t even_size = is_odd ? (nodes.size()-1) : nodes.size();
    for (size_t i = 0; i < even_size; i += 2) {
      Node* left = nodes.front();
      nodes.pop_front();
      Node* right = nodes.front();
      nodes.pop_front();
      nodes.push_back(new Node(left, right));
    }
    if (is_odd) {
      nodes.push_back(new Node(nodes.front()));
      nodes.pop_front();
    }
    return nodes.size();
  }

  bool Verify_(Node* node) const {
    return node == nullptr ||
              (node->Verify() &&
                (node->IsLeaf() ||
                  (Verify_(node->left) && Verify_(node->right))));
  }

};

} // namespace merkletree