// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <memory>

#include "CircularQueue.h"
#include "gtest/gtest.h"
#include "stubs/mbed/mbed_critical.h"

using namespace leka;

class CircularQueueTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		spy_mbed_critical_enter_calls = 0;
		spy_mbed_critical_exit_calls  = 0;
	}
	// void TearDown() override {}

	static constexpr auto TEST_BUFFER_SIZE {10};
	CircularQueue<int, TEST_BUFFER_SIZE> buf {};
};
