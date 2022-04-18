// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, peekOneItem)
{
	buf.push(42);
	EXPECT_EQ(buf.size(), 1);

	int item = 0;
	bool ret = buf.peek(item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 42);
	EXPECT_EQ(buf.size(), 1);
}

TEST_F(CircularQueueTest, peekOneItemWhenEmpty)
{
	EXPECT_EQ(buf.size(), 0);

	int item = 0;
	bool ret = buf.peek(item);

	EXPECT_FALSE(ret);
	EXPECT_EQ(item, 0);
	EXPECT_EQ(buf.size(), 0);
}

TEST_F(CircularQueueTest, peekOneItemAtPosition)
{
	buf.push(42);
	buf.push(43);
	buf.push(44);

	EXPECT_EQ(buf.size(), 3);

	int item = 0;
	bool ret = false;

	ret = buf.peekAt(0, item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 42);
	EXPECT_EQ(buf.size(), 3);

	ret = buf.peekAt(1, item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 43);
	EXPECT_EQ(buf.size(), 3);

	ret = buf.peekAt(2, item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 44);
	EXPECT_EQ(buf.size(), 3);
}

TEST_F(CircularQueueTest, peekOneItemAtPositionLoopOver)
{
	CircularQueue<int, 4> buff {};

	// ? push/pop to create offset

	buff.push(1);
	buff.push(2);
	buff.push(3);

	EXPECT_EQ(buff.size(), 3);

	buff.drop();
	buff.drop();
	buff.drop();

	EXPECT_EQ(buff.size(), 0);

	buff.push(42);
	buff.push(43);
	buff.push(44);

	EXPECT_EQ(buff.size(), 3);

	{
		int item = 0;
		bool ret = false;

		ret = buff.peekAt(0, item);

		EXPECT_TRUE(ret);
		EXPECT_EQ(item, 42);
		EXPECT_EQ(buff.size(), 3);
	}

	{
		int item = 0;
		bool ret = false;

		ret = buff.peekAt(1, item);

		EXPECT_TRUE(ret);
		EXPECT_EQ(item, 43);
		EXPECT_EQ(buff.size(), 3);
	}

	{
		int item = 0;
		bool ret = false;

		ret = buff.peekAt(2, item);

		EXPECT_TRUE(ret);
		EXPECT_EQ(item, 44);
		EXPECT_EQ(buff.size(), 3);
	}
}

TEST_F(CircularQueueTest, peekOneItemAtPositionWhenEmpty)
{
	EXPECT_EQ(buf.size(), 0);

	int item = 0;
	bool ret = false;

	ret = buf.peekAt(0, item);

	EXPECT_FALSE(ret);
	EXPECT_EQ(item, 0);

	ret = buf.peekAt(1, item);

	EXPECT_FALSE(ret);
	EXPECT_EQ(item, 0);
}

TEST_F(CircularQueueTest, peekOneItemAtPositionBiggerThanSize)
{
	buf.push(42);
	buf.push(43);
	buf.push(44);

	EXPECT_EQ(buf.size(), 3);

	int item = 0;
	bool ret = false;

	ret = buf.peekAt(3, item);

	EXPECT_FALSE(ret);
	EXPECT_EQ(item, 0);

	ret = buf.peekAt(4, item);

	EXPECT_FALSE(ret);
	EXPECT_EQ(item, 0);
}
