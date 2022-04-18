// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, isEmptyAfterInit)
{
	CircularQueue<int, 5> local_buf {};

	auto is_empty = local_buf.empty();

	EXPECT_TRUE(is_empty);
}

TEST_F(CircularQueueTest, isEmptyWhenEmpty)
{
	auto is_empty = buf.empty();

	EXPECT_TRUE(is_empty);
}

TEST_F(CircularQueueTest, isEmptyWhenNotEmpty)
{
	buf.push(1);
	auto is_empty = buf.empty();

	EXPECT_FALSE(is_empty);
}

TEST_F(CircularQueueTest, isEmptyWhenFull)
{
	auto items = std::array<int, TEST_BUFFER_SIZE> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	buf.push(items.data(), std::size(items));

	auto is_empty = buf.empty();

	EXPECT_FALSE(is_empty);
}
