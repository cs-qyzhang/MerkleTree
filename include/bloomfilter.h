#pragma once

#include <bitset>
#include <utility>

namespace merkletree {

template<typename T, size_t bitset_len = 10000>
class BloomFilter {
 public:
  BloomFilter() = default;

  BloomFilter(const std::vector<T*> data) {
    for (auto& d : data)
      Add(d);
  }

  BloomFilter(const BloomFilter& a) {
    set_ = a.set_;
  }

  BloomFilter(const BloomFilter& a, const BloomFilter& b) {
    set_ = a.set_ | b.set_;
  }

  void Add(T* data) {
    size_t hash_val = std::hash<T>{}(*data);
    set_[hash_val%bitset_len] = true;
  }

  bool MaybeExist(T* data) const {
    size_t hash_val = std::hash<T>{}(*data);
    return set_[hash_val%bitset_len];
  }

  double FillRate() const {
    return (double)set_.count() / (double)bitset_len;
  }

 private:
  std::bitset<bitset_len> set_;
};

} // namespace merkletree
