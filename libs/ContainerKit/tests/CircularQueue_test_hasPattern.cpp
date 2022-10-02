// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, hasPattern)
{
	auto items	 = std::array {0, 1, 2, 0x2A, 0x2B, 0x2C, 0x2D, 7, 8, 9};
	auto pattern = std::array {0x2A, 0x2B, 0x2C, 0x2D};

	buf.push(items.data(), std::size(items));

	auto pos = uint8_t {};

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_TRUE(ret);
	EXPECT_EQ(pos, 3);
}

TEST_F(CircularQueueTest, hasPatternSpan)
{
	auto items	 = std::array {0, 1, 2, 0x2A, 0x2B, 0x2C, 0x2D, 7, 8, 9};
	auto pattern = std::array {0x2A, 0x2B, 0x2C, 0x2D};

	buf.push(items);

	auto pos = uint8_t {};

	auto ret = buf.hasPattern(pattern, pos);

	EXPECT_TRUE(ret);
	EXPECT_EQ(pos, 3);
}

TEST_F(CircularQueueTest, hasPatternLoopOver)
{
	auto items	 = std::array {0, 1, 2, 3, 4, 5, 6, 7, 0x2A, 0x2B, 0x2C, 0x2D};
	auto pattern = std::array {0x2A, 0x2B, 0x2C, 0x2D};

	buf.push(items.data(), std::size(items));

	auto pos = uint8_t {};

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_TRUE(ret);
	EXPECT_EQ(pos, 6);
}

TEST_F(CircularQueueTest, hasNotPattern)
{
	auto items	 = std::array {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto pattern = std::array {0x2A, 0x2B, 0x2C, 0x2D};

	buf.push(items.data(), std::size(items));

	auto pos = uint8_t {};

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_FALSE(ret);
	EXPECT_EQ(pos, 0);
}

TEST_F(CircularQueueTest, hasOnlyPartOfPattern)
{
	auto items	 = std::array {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto pattern = std::array {2, 3, 4, 0x2D};

	buf.push(items.data(), std::size(items));

	auto pos = uint8_t {};

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_FALSE(ret);
	EXPECT_EQ(pos, 0);
}
