// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, drop)
{
	auto items = std::array<int, TEST_BUFFER_SIZE> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	buf.push(items.data(), std::size(items));

	buf.drop();

	EXPECT_EQ(buf.size(), TEST_BUFFER_SIZE - 1);

	buf.drop();
	buf.drop();
	buf.drop();
	buf.drop();

	EXPECT_EQ(buf.size(), TEST_BUFFER_SIZE - 5);

	buf.drop();
	buf.drop();
	buf.drop();
	buf.drop();
	buf.drop();

	EXPECT_EQ(buf.size(), TEST_BUFFER_SIZE - 10);
}

TEST_F(CircularQueueTest, dropWhenEmpty)
{
	EXPECT_EQ(buf.size(), 0);

	buf.drop();

	EXPECT_EQ(buf.size(), 0);

	buf.drop();
	buf.drop();
	buf.drop();
	buf.drop();
	buf.drop();

	EXPECT_EQ(buf.size(), 0);
}
