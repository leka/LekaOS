// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string>

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/stub_Kernel.h"

using namespace leka;

using ::testing::HasSubstr;
using ::testing::MatchesRegex;

class LogKitNowTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_string = "";
		logger::set_print_function(test_printf);
	}

	void TearDown() override
	{
		logger::set_print_function(logger::default_printf);
		logger::set_now_function(logger::default_now);
	}

	static void test_printf(const char *str, size_t size)
	{
		spy_string = std::string {str};
		std::cout << spy_string;
	}

	static auto test_dynamic_now() -> int64_t
	{
		spy_now = std::chrono::system_clock::now().time_since_epoch().count();
		return spy_now;
	}

	static inline auto spy_string = std::string {};
	static inline auto spy_now	  = int64_t {0};
};

TEST_F(LogKitNowTest, useDefaultNowFunction)
{
	ASSERT_EQ(spy_kernel_tick_count, logger::now());
}

TEST_F(LogKitNowTest, setConstNowFunction)
{
	logger::set_now_function([]() { return int64_t {1619354279231635}; });

	ASSERT_EQ(1619354279231635, leka::logger::now());
}

TEST_F(LogKitNowTest, setDynamicNowFunction)
{
	logger::set_now_function(test_dynamic_now);

	auto expected = logger::now();

	ASSERT_EQ(expected, spy_now);
}
