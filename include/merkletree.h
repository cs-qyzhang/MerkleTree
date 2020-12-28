#pragma once

#include <cstddef>
#include <utility>
#include <functional>
#include <memory>
#include <vector>
#include <deque>
#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>
#include "bloomfilter.h"
#include "config.h"

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

template< typename T = std::string, size_t bloom_filter_bits = 10000, typename Hash = SHA256Hash >
class MerkleTree {
 public:
  MerkleTree() : root_(nullptr), nr_leaf_(0), byte_use_(0), level_(0) {}
  MerkleTree(const std::vector<T*>& data) { BuildFromVector(data); }

  void BuildFromVector(const std::vector<T*>& data) {
    byte_use_ = 0;
    std::deque<Node*> nodes;
    for (auto d : data)
      nodes.emplace_back(new Node(d));
    byte_use_ += nodes.size() * sizeof(Node);
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
  size_t ByteUse() const { return byte_use_; }

  bool Exist(T* data) const {
    Hash data_hash(data);
    return Exist_(root_, data_hash, data);
  }

#ifdef BLOOM
  double BloomFillRate() const { return root_->filter.FillRate(); }
#endif

 private:
  struct Node {
    Node*  left;    // nullptr if is leaf
    union {
      Node*  right;
      T*     data;
    };
    Hash hash;
#ifdef BLOOM
    BloomFilter<T, bloom_filter_bits> filter;
#endif

    Node(T* d) : left(nullptr), data(d), hash(data) {
#ifdef BLOOM
      filter.Add(d);
#endif
    }
    Node(Node* l) : left(l), right(nullptr), hash(l->hash)
#ifdef BLOOM
      , filter(l->filter)
#endif
      {}
    Node(Node* l, Node* r) :left(l), right(r), hash(l->hash, r->hash)
#ifdef BLOOM
      , filter(l->filter, r->filter)
#endif
      {}

    bool IsLeaf() const { return left == nullptr; }
    T* Data() const { return (T*)data; }

    bool Verify() const {
      return IsLeaf() ? hash == Hash(data) : hash == Hash(left->hash, right->hash);
    }
  };

  Node*  root_;
  size_t nr_leaf_;
  size_t byte_use_;
  int    level_;

  size_t BuildUpperLevel_(std::deque<Node*>& nodes) {
    int is_odd = nodes.size() % 2;
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
    byte_use_ += (even_size/2 + is_odd) * sizeof(Node);
    return nodes.size();
  }

  bool Exist_(Node* node, Hash& target_hash, T* data) const {
    if (node == nullptr)
      return false;
#ifdef BLOOM
    if (!node->filter.MaybeExist(data))
      return false;
#endif
    if (node->IsLeaf())
      return (node->hash == target_hash) && (*data == *node->data);
    else
      return Exist_(node->left, target_hash, data) || Exist_(node->right, target_hash, data);
  }

  bool Verify_(Node* node) const {
    return node == nullptr ||
              (node->Verify() &&
                (node->IsLeaf() ||
                  (Verify_(node->left) && Verify_(node->right))));
  }

};

} // namespace merkletree