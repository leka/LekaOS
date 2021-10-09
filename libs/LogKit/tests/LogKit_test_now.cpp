// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string>

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/mbed/Kernel.h"

using namespace leka;

using ::testing::HasSubstr;
using ::testing::MatchesRegex;

class LogKitNowTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_string = "";
		spy_now	   = 0;
	}

	void TearDown() override { logger::set_now_function(logger::default_now); }

	static auto dynamic_now() -> int64_t
	{
		spy_now = std::chrono::system_clock::now().time_since_epoch().count();
		return spy_now;
	}

	static inline auto spy_string = std::string {};
	static inline auto spy_now	  = int64_t {};
};

TEST_F(LogKitNowTest, useDefaultNowFunction)
{
	ASSERT_EQ(spy_kernel_tick_count, logger::now());
}

TEST_F(LogKitNowTest, setConstNowFunction)
{
	auto const_now = []() { return int64_t {1619354279231635}; };

	logger::set_now_function(const_now);

	ASSERT_EQ(1619354279231635, leka::logger::now());
}

TEST_F(LogKitNowTest, setDynamicNowFunction)
{
	logger::set_now_function(dynamic_now);

	auto expected = logger::now();

	ASSERT_EQ(expected, spy_now);
}
