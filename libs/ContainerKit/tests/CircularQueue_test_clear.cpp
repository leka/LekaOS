// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, clearAfterPushOne)
{
	buf.push(1);
	EXPECT_EQ(buf.size(), 1);

	buf.clear();
	EXPECT_EQ(buf.size(), 0);
}

TEST_F(CircularQueueTest, clearAfterPushOneMultiple)
{
	buf.push(1);
	buf.push(2);
	buf.push(3);
	EXPECT_EQ(buf.size(), 3);

	buf.clear();
	EXPECT_EQ(buf.size(), 0);
}

TEST_F(CircularQueueTest, clearAfterPushMultiple)
{
	auto data = {1, 2, 3, 4, 5};

	buf.push(data.begin(), data.size());

	EXPECT_EQ(buf.size(), data.size());

	buf.clear();
	EXPECT_EQ(buf.size(), 0);
}
