// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "../include/CircularBuffer.h"
#include <array>
#include <memory>

#include "gtest/gtest.h"

using namespace leka::utils;

class CircularBufferTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr auto TEST_BUFFER_SIZE {10};
	CircularBuffer<int, TEST_BUFFER_SIZE> buf {};
};

TEST_F(CircularBufferTest, initialization)
{
	ASSERT_NE(&buf, nullptr);
}

TEST_F(CircularBufferTest, pushOneItemPopOneItem)
{
	int item = 0;

	buf.push(1);
	bool ret = buf.pop(item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 1);
}

TEST_F(CircularBufferTest, reset)
{
	buf.push(1);
	EXPECT_EQ(buf.size(), 1);

	buf.reset();
	EXPECT_EQ(buf.size(), 0);
}

TEST_F(CircularBufferTest, popOneItemWhenEmpty)
{
	int item = 0;
	bool ret = buf.pop(item);
	EXPECT_FALSE(ret);
}

TEST_F(CircularBufferTest, popMutipleItemsWhenEmpty)
{
	std::array<int, 3> items {};
	bool ret = buf.pop(items.data(), std::size(items));
	EXPECT_FALSE(ret);
}

TEST_F(CircularBufferTest, pushPopMultipleItems)
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

TEST_F(CircularBufferTest, pushOneItemToMakeFull)
{
	auto items = std::array<int, TEST_BUFFER_SIZE - 1> {1, 2, 3, 4, 5, 6, 7, 8, 9};

	buf.push(items.data(), std::size(items));

	EXPECT_FALSE(buf.full());

	buf.push(10);

	auto expected_items = std::array<int, TEST_BUFFER_SIZE> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto actual_items	= std::array<int, TEST_BUFFER_SIZE> {};

	auto ret = buf.pop(actual_items.data(), std::size(actual_items));

	EXPECT_TRUE(buf.empty());
	EXPECT_FALSE(buf.full());
	EXPECT_EQ(actual_items, expected_items);
}

TEST_F(CircularBufferTest, pushOneItemWhenAlreadyFull)
{
	auto items = std::array<int, TEST_BUFFER_SIZE - 1> {1, 2, 3, 4, 5, 6, 7, 8, 9};

	buf.push(items.data(), std::size(items));

	EXPECT_FALSE(buf.full());

	buf.push(10);

	EXPECT_TRUE(buf.full());

	buf.push(11);

	auto expected_items = std::array<int, TEST_BUFFER_SIZE> {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	auto actual_items	= std::array<int, TEST_BUFFER_SIZE> {};

	auto ret = buf.pop(actual_items.data(), std::size(actual_items));

	EXPECT_TRUE(buf.empty());
	EXPECT_FALSE(buf.full());
	EXPECT_EQ(actual_items, expected_items);
}

TEST_F(CircularBufferTest, pushItemsWithOverflow)
{
	auto items		  = std::array<int, TEST_BUFFER_SIZE> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto items_popped = std::array<int, TEST_BUFFER_SIZE> {};

	buf.push(-1);

	// there is now not enough space for all the elements, old ones should be overwritten

	buf.push(items.data(), TEST_BUFFER_SIZE);

	int number_of_items = buf.pop(items_popped.data(), TEST_BUFFER_SIZE);
	EXPECT_EQ(number_of_items, TEST_BUFFER_SIZE);
	EXPECT_TRUE(0 == memcmp(items.data(), items_popped.data(), TEST_BUFFER_SIZE));

	// there is a difference where the overflow is caused by a smaller write
	// and the buffer should retain part of old values

	buf.push(-1);
	buf.push(-2);
	buf.push(items.data(), TEST_BUFFER_SIZE - 1);	// -1 is overwritten but -2 is kept

	int popped_number;
	buf.pop(popped_number);
	EXPECT_EQ(popped_number, -2);

	buf.pop(items_popped.data(), TEST_BUFFER_SIZE - 1);
	EXPECT_TRUE(0 == memcmp(items.data(), items_popped.data(), TEST_BUFFER_SIZE - 1));
}

TEST_F(CircularBufferTest, pushMoreItemsThanBufferCapacity)
{
	auto items		  = std::array<int, TEST_BUFFER_SIZE + 1> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	auto items_popped = std::array<int, TEST_BUFFER_SIZE> {};

	// the loop creates different amounts of existing elements prior to write over capacity
	for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
		for (int j = 0; j < i; j++) {
			buf.push(-1);
		}
		// first element should be dropped
		buf.push(items.data(), TEST_BUFFER_SIZE + 1);

		int number_of_items = buf.pop(items_popped.data(), TEST_BUFFER_SIZE + 1);
		EXPECT_EQ(number_of_items, TEST_BUFFER_SIZE);
		EXPECT_EQ(buf.size(), 0);
		EXPECT_TRUE(0 == memcmp(items.data() + 1, items_popped.data(), TEST_BUFFER_SIZE));
	}
}
