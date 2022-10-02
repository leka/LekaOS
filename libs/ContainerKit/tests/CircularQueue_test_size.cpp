// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, sizeAfterInit)
{
	CircularQueue<int, 5> local_buf {};

	auto size = local_buf.size();

	EXPECT_EQ(size, 0);
}

TEST_F(CircularQueueTest, sizeWhenEmpty)
{
	auto size = buf.size();

	EXPECT_EQ(size, 0);
}

TEST_F(CircularQueueTest, sizeWhenNotEmpty)
{
	buf.push(1);
	auto size = buf.size();

	EXPECT_EQ(size, 1);
}

TEST_F(CircularQueueTest, sizeWhenFull)
{
	auto items = std::array<int, TEST_BUFFER_SIZE> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	buf.push(items.data(), std::size(items));

	auto size = buf.size();

	EXPECT_EQ(size, TEST_BUFFER_SIZE);
}
