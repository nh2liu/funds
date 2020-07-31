#pragma once
#include <bitset>

namespace funds {

// Variadic base case.
template <size_t N, typename Key, typename... Hashes> class bloom {
protected:
  std::bitset<N> b_;
  void insert(const Key &k) {}
  bool query(const Key &k) const { return true; }
};

// Classic Bloom Filter Implementation that can take arbitrary hash functions.
// TODO: Add type traits for Hash and Key types since Hash must be a callable
// from Key -> size_t.
template <size_t N, typename Key, typename Hash, typename... Hashes>
class bloom<N, Key, Hash, Hashes...> : public bloom<N, Key, Hashes...> {
  Hash h_;
  inline size_t index(const Key &k) const { return h_(k) % N; }

public:
  bloom(Hash h, Hashes... hashes)
      : bloom<N, Key, Hashes...>(hashes...), h_(h) {}

  void insert(const Key &k) {
    this->b_.set(index(k));
    bloom<N, Key, Hashes...>::insert(k);
  }

  bool query(const Key &k) const {
    return this->b_.test(index(k)) && bloom<N, Key, Hashes...>::query(k);
  }
};

/*
  The function below is so an api without entering the hash types in the
  template can be used.
  Eg. auto b = make_bloom<1024, uint32_t>(hash1, hash2, hash3);
*/
template <size_t N, typename Key, typename... Hashes>
constexpr auto make_bloom(Hashes... hashes) {
  return bloom<N, Key, Hashes...>(hashes...);
}

} // namespace funds
