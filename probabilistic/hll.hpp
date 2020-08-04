#pragma once
#include <algorithm>
#include <array>
#include <cmath>
namespace funds {

constexpr static double get_bias_var(size_t B) {
  if (B < 4) {
    return 0.6;
  } else if (B == 4) {
    return 0.673;
  } else if (B == 5) {
    return 0.697;
  } else if (B == 6) {
    return 0.709;
  } else {
    return 0.7213 / (1 + 1.079 / std::pow(2, B));
  }
}

// A hyperloglog implementation with linear counting at low levels.
// TODO: Add linear counting.
template <size_t B, typename Key, typename Hash, typename HashOutput = uint32_t>
class hll {
  constexpr static HashOutput N_REGISTERS = 1 << B;
  constexpr static HashOutput BITS_RESERVED = sizeof(HashOutput) * 8 - B;
  std::array<char, N_REGISTERS> store_;
  Hash h_;
  constexpr static double BIAS_ADJUSTMENT = get_bias_var(B);

  using hash_t = HashOutput;
  char index_(hash_t prefix_key) {
    char r = 0;
    while (prefix_key >>= 1) {
      r++;
    }
    return BITS_RESERVED - r;
  }

  char harmonic_mean() const {
    double power_sum_ = 0;
    for (double d : store_) {
      power_sum_ += std::pow(2, -d);
    }
    return 1. / power_sum_;
  }

public:
  hll(Hash h) : h_{h} {}

  void insert(const Key &key) {
    size_t x = h_(key);
    hash_t reg = x >> B;
    hash_t prefix_key = x & (1 << BITS_RESERVED - 1);
    store_[reg] = max(store_[reg], index_(prefix_key));
  }

  double count() const {
    return BIAS_ADJUSTMENT * N_REGISTERS * N_REGISTERS * harmonic_mean();
  }
};

} // namespace funds
