// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "../include/CircularBuffer.h"
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

TEST_F(CircularBufferTest, constructor)
{
	ASSERT_NE(&buf, nullptr);
}

TEST_F(CircularBufferTest, push_pop)
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

TEST_F(CircularBufferTest, pop_empty)
{
	int item = 0;
	bool ret = buf.pop(item);
	EXPECT_FALSE(ret);
}

TEST_F(CircularBufferTest, push_pop_multiple)
{
	const int test_numbers[TEST_BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	/* this will check pushing across the buffer end */
	for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
		int test_numbers_popped[TEST_BUFFER_SIZE] = {0};
		buf.push(test_numbers, i);
		EXPECT_EQ(buf.size(), i);
		int number_of_items = buf.pop(test_numbers_popped, i);
		EXPECT_EQ(buf.size(), 0);
		EXPECT_EQ(number_of_items, i);
		EXPECT_TRUE(0 == memcmp(test_numbers, test_numbers_popped, i));
	}
}

TEST_F(CircularBufferTest, overflow)
{
	const int test_numbers[TEST_BUFFER_SIZE]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int test_numbers_popped[TEST_BUFFER_SIZE] = {0};

	buf.push(-1);

	/* there is now not enough space for all the elements, old ones should be overwritten */

	buf.push(test_numbers, TEST_BUFFER_SIZE);

	int number_of_items = buf.pop(test_numbers_popped, TEST_BUFFER_SIZE);
	EXPECT_EQ(number_of_items, TEST_BUFFER_SIZE);
	EXPECT_TRUE(0 == memcmp(test_numbers, test_numbers_popped, TEST_BUFFER_SIZE));

	/* there is a difference where the overflow is caused by a smaller write
	 * and the buffer should retain part of old values */

	buf.push(-1);
	buf.push(-2);
	buf.push(test_numbers, TEST_BUFFER_SIZE - 1); /* -1 is overwritten but -2 is kept */

	int popped_number;
	buf.pop(popped_number);
	EXPECT_EQ(popped_number, -2);

	buf.pop(test_numbers_popped, TEST_BUFFER_SIZE - 1);
	EXPECT_TRUE(0 == memcmp(test_numbers, test_numbers_popped, TEST_BUFFER_SIZE - 1));
}

TEST_F(CircularBufferTest, writing_over_max_capacity)
{
	const int test_numbers[TEST_BUFFER_SIZE + 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	int test_numbers_popped[TEST_BUFFER_SIZE]	 = {0};

	/* the loop creates different amounts of existing elements prior to write over capacity */
	for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
		for (int j = 0; j < i; j++) {
			buf.push(-1);
		}
		/* first element should be dropped */
		buf.push(test_numbers, TEST_BUFFER_SIZE + 1);

		int number_of_items = buf.pop(test_numbers_popped, TEST_BUFFER_SIZE + 1);
		EXPECT_EQ(number_of_items, TEST_BUFFER_SIZE);
		EXPECT_EQ(buf.size(), 0);
		EXPECT_TRUE(0 == memcmp(test_numbers + 1, test_numbers_popped, TEST_BUFFER_SIZE));
	}
}
