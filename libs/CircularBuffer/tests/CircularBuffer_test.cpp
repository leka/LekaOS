// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "../include/CircularBuffer.h"
#include <array>
#include <memory>

#include "gtest/gtest.h"
#include "stub_mbed_critical.h"

using namespace leka;

class CircularBufferTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		spy_mbed_critical_enter_calls = 0;
		spy_mbed_critical_exit_calls  = 0;
	}
	// void TearDown() override {}

	static constexpr auto TEST_BUFFER_SIZE {10};
	CircularBuffer<int, TEST_BUFFER_SIZE> buf {};
};

TEST_F(CircularBufferTest, initialization)
{
	EXPECT_NE(&buf, nullptr);
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

TEST_F(CircularBufferTest, sizeWhenEmpty)
{
	auto size = buf.size();

	EXPECT_EQ(size, 0);
}

TEST_F(CircularBufferTest, sizeWhenNotEmpty)
{
	buf.push(1);
	auto size = buf.size();

	EXPECT_EQ(size, 1);
}

TEST_F(CircularBufferTest, sizeWhenFull)
{
	auto items = std::array<int, TEST_BUFFER_SIZE> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	buf.push(items.data(), std::size(items));

	auto size = buf.size();

	EXPECT_EQ(size, TEST_BUFFER_SIZE);
}

TEST_F(CircularBufferTest, isEmptyWhenEmpty)
{
	auto is_empty = buf.empty();

	EXPECT_TRUE(is_empty);
}

TEST_F(CircularBufferTest, isEmptyWhenNotEmpty)
{
	buf.push(1);
	auto is_empty = buf.empty();

	EXPECT_FALSE(is_empty);
}

TEST_F(CircularBufferTest, isEmptyWhenFull)
{
	auto items = std::array<int, TEST_BUFFER_SIZE> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	buf.push(items.data(), std::size(items));

	auto is_empty = buf.empty();

	EXPECT_FALSE(is_empty);
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

TEST_F(CircularBufferTest, peekOneItem)
{
	buf.push(42);
	EXPECT_EQ(buf.size(), 1);

	int item = 0;
	bool ret = buf.peek(item);

	EXPECT_TRUE(ret);
	EXPECT_EQ(item, 42);
	EXPECT_EQ(buf.size(), 1);
}

TEST_F(CircularBufferTest, peekOneItemWhenEmpty)
{
	EXPECT_EQ(buf.size(), 0);

	int item = 0;
	bool ret = buf.peek(item);

	EXPECT_FALSE(ret);
	EXPECT_EQ(item, 0);
	EXPECT_EQ(buf.size(), 0);
}

TEST_F(CircularBufferTest, peekOneItemAtPosition)
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

TEST_F(CircularBufferTest, peekOneItemAtPositionWhenEmpty)
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

TEST_F(CircularBufferTest, peekOneItemAtPositionBiggerThanSize)
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

TEST_F(CircularBufferTest, criticalSectionForPush)
{
	buf.push(1);

	EXPECT_TRUE(spy_mbed_critical_enter_was_called());
	EXPECT_TRUE(spy_mbed_critical_exit_was_called());
}

TEST_F(CircularBufferTest, criticalSectionForPop)
{
	int item = 0;
	bool ret = buf.pop(item);

	EXPECT_TRUE(spy_mbed_critical_enter_was_called());
	EXPECT_TRUE(spy_mbed_critical_exit_was_called());
}

TEST_F(CircularBufferTest, hasPattern)
{
	auto items	 = std::array {0, 1, 2, 0x2A, 0x2B, 0x2C, 0x2D, 7, 8, 9};
	auto pattern = std::array {0x2A, 0x2B, 0x2C, 0x2D};

	buf.push(items.data(), std::size(items));

	int pos = 0;

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_TRUE(ret);
	EXPECT_EQ(pos, 3);
}

TEST_F(CircularBufferTest, hasNotPattern)
{
	auto items	 = std::array {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto pattern = std::array {0x2A, 0x2B, 0x2C, 0x2D};

	buf.push(items.data(), std::size(items));

	int pos = 0;

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_FALSE(ret);
	EXPECT_EQ(pos, 0);
}

TEST_F(CircularBufferTest, hasOnlyPartOfPattern)
{
	auto items	 = std::array {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto pattern = std::array {2, 3, 4, 0x2D};

	buf.push(items.data(), std::size(items));

	int pos = 0;

	auto ret = buf.hasPattern(pattern.data(), std::size(pattern), pos);

	EXPECT_FALSE(ret);
	EXPECT_EQ(pos, 0);
}
