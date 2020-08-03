#include "probabilistic/bloom.hpp"
#include <gtest/gtest.h>
using namespace funds;

// Two sample hash functions.
auto hash1 = [](uint32_t v) { return v; };

auto hash2 = [](uint32_t v) { return v * 2; };

struct Functor1 {
  uint32_t operator()(uint32_t v) const { return v; }
};

struct Functor2 {
  uint32_t operator()(uint32_t v) const { return v * 2; }
};

TEST(BloomTest, DefaultConstruction) {
  auto b = bloom<10, uint32_t, Functor1, Functor2>();
  EXPECT_FALSE(b.query(4));
  EXPECT_FALSE(b.query(3));
  b.insert(4);
  EXPECT_TRUE(b.query(4));
  EXPECT_FALSE(b.query(2));
  EXPECT_FALSE(b.query(3));
  b.insert(3);
  EXPECT_TRUE(b.query(3));
  // Both hash bits should be true under size 10.
  EXPECT_TRUE(b.query(33));
  EXPECT_FALSE(b.query(37));
}

TEST(BloomTest, CommonMethodCallSequence) {
  auto b = make_bloom<10, uint32_t>(hash1, hash2);
  EXPECT_FALSE(b.query(4));
  EXPECT_FALSE(b.query(3));
  b.insert(4);
  EXPECT_TRUE(b.query(4));
  EXPECT_FALSE(b.query(2));
  EXPECT_FALSE(b.query(3));
  b.insert(3);
  EXPECT_TRUE(b.query(3));
  // Both hash bits should be true under size 10.
  EXPECT_TRUE(b.query(33));
  EXPECT_FALSE(b.query(37));
}

TEST(BloomTest, Reset) {
  auto b = make_bloom<10, uint32_t>(hash1, hash2);
  EXPECT_FALSE(b.query(4));
  b.insert(4);
  EXPECT_TRUE(b.query(4));
  b.reset();
  EXPECT_FALSE(b.query(4));
  b.insert(4);
  EXPECT_TRUE(b.query(4));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
