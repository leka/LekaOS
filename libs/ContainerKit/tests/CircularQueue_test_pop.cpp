// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, popOneItemWhenEmpty)
{
	int item = 0;
	bool ret = buf.pop(item);
	EXPECT_FALSE(ret);
}

TEST_F(CircularQueueTest, popMutipleItemsWhenEmpty)
{
	std::array<int, 3> items {};
	bool ret = buf.pop(items.data(), std::size(items));
	EXPECT_FALSE(ret);
}
