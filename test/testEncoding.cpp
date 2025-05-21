#include <gtest/gtest.h>
#include "ZigZagVectors.h"
#include "ImageBlock.h"


TEST(EncodeACTest, HandlesAllZeros) {
std::vector<char> input(63, 0);  // 63 zeros
auto result = ZigZagVectors::encodeAC(input);
ASSERT_EQ(result.size(), 1);
EXPECT_EQ(result[0], std::make_pair(0u, (char)0));  // EOB
}

TEST(EncodeACTest, HandlesSingleValueAtStart) {
std::vector<char> input(63, 0);
input[0] = 5;  // Non-zero right at start
auto result = ZigZagVectors::encodeAC(input);

ASSERT_EQ(result.size(), 2);
EXPECT_EQ(result[0], std::make_pair(0u, (char)5));
EXPECT_EQ(result[1], std::make_pair(0u, (char)0));  // EOB
}

TEST(EncodeACTest, HandlesZRLInsertions) {
std::vector<char> input(63, 0);
input[48] = 10;  // 48 zeros before non-zero

auto result = ZigZagVectors::encodeAC(input);

ASSERT_EQ(result.size(), 5);
EXPECT_EQ(result[0], std::make_pair(15u, (char)0));
EXPECT_EQ(result[1], std::make_pair(15u, (char)0));
EXPECT_EQ(result[2], std::make_pair(15u, (char)0));
EXPECT_EQ(result[3], std::make_pair(0u, (char)10));
}