// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "./CircularQueue_test.h"

TEST_F(CircularQueueTest, initialization)
{
	EXPECT_NE(&buf, nullptr);
}

TEST_F(CircularQueueTest, criticalSectionForPush)
{
	buf.push(1);

	EXPECT_TRUE(spy_mbed_critical_enter_was_called());
	EXPECT_TRUE(spy_mbed_critical_exit_was_called());
}

TEST_F(CircularQueueTest, criticalSectionForPop)
{
	int item = 0;
	bool ret = buf.pop(item);

	EXPECT_TRUE(spy_mbed_critical_enter_was_called());
	EXPECT_TRUE(spy_mbed_critical_exit_was_called());
}
