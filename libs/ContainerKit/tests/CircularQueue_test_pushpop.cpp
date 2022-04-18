// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, pushOneItemPopOneItem)
{
	int item = 0;

	buf.push(1);
	bool ret = buf.pop(item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 1);
}

TEST_F(CircularQueueTest, pushPopMultipleItemsSpan)
{
	auto items		  = std::array<int, TEST_BUFFER_SIZE> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto items_popped = std::array<int, TEST_BUFFER_SIZE> {};

	buf.push(items);

	EXPECT_EQ(buf.size(), 10);

	int number_of_items = buf.pop(items_popped.data(), 10);

	EXPECT_EQ(buf.size(), 0);
	EXPECT_EQ(number_of_items, 10);
	EXPECT_TRUE(0 == memcmp(items.data(), items_popped.data(), 10));
}

TEST_F(CircularQueueTest, pushPopMultipleItemsSpanTwice)
{
	auto items		  = std::array<int, TEST_BUFFER_SIZE> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto items_1	  = std::array<int, 5> {1, 2, 3, 4, 5};
	auto items_2	  = std::array<int, 5> {6, 7, 8, 9, 10};
	auto items_popped = std::array<int, TEST_BUFFER_SIZE> {};

	{
		buf.push(items_1);

		EXPECT_EQ(buf.size(), 5);

		int number_of_items = buf.pop(items_popped.data(), 5);

		EXPECT_EQ(buf.size(), 0);
		EXPECT_EQ(number_of_items, 5);
		EXPECT_TRUE(0 == memcmp(items_1.data(), items_popped.data(), 5));
	}

	{
		buf.push(items_1);
		buf.push(items_2);

		EXPECT_EQ(buf.size(), 10);

		int number_of_items = buf.pop(items_popped.data(), 10);

		EXPECT_EQ(buf.size(), 0);
		EXPECT_EQ(number_of_items, 10);
		EXPECT_TRUE(0 == memcmp(items.data(), items_popped.data(), 10));
	}
}

TEST_F(CircularQueueTest, pushPopMultipleItems)
{
	auto items = std::array<int, TEST_BUFFER_SIZE> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
		auto items_popped = std::array<int, TEST_BUFFER_SIZE> {};

		buf.push(items.data(), i);

		EXPECT_EQ(buf.size(), i);

		int number_of_items = buf.pop(items_popped.data(), i);

		EXPECT_EQ(buf.size(), 0);
		EXPECT_EQ(number_of_items, i);
		EXPECT_TRUE(0 == memcmp(items.data(), items_popped.data(), i));
	}
}

TEST_F(CircularQueueTest, pushPopMultipleItemsLoopOver)
{
	auto items_offset = std::array<int, 4> {42, 42, 42, 42};
	auto items		  = std::array<int, TEST_BUFFER_SIZE> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	// ? push/pop to create offset
	buf.push(items_offset.data(), items_offset.size());
	buf.pop(items_offset.data(), items_offset.size());

	for (int i = 0; i < TEST_BUFFER_SIZE; ++i) {
		auto items_popped = std::array<int, TEST_BUFFER_SIZE> {};

		buf.push(items.data(), i);

		EXPECT_EQ(buf.size(), i);

		int number_of_items = buf.pop(items_popped.data(), i);

		EXPECT_EQ(buf.size(), 0);
		EXPECT_EQ(number_of_items, i);
		EXPECT_TRUE(0 == memcmp(items.data(), items_popped.data(), i));
	}
}
