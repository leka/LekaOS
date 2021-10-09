// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::HasSubstr;

using namespace leka;

class LogKitTraceTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_string = "";
		logger::set_trace_function(test_trace);
	}

	void TearDown() override { logger::set_trace_function(logger::default_trace_function); }

	static void test_trace(const char *str, size_t size)
	{
		spy_string = "Custom print function: " + std::string {str};
		std::cout << spy_string;
	}

	static void test_default_trace(const char *str, size_t size)
	{
		spy_string = std::string {str};
		logger::default_trace_function(str, size);
	}

	static inline auto spy_string = std::string {};
};

TEST_F(LogKitTraceTest, useDefaultTraceFunction)
{
	logger::set_trace_function(test_default_trace);

	log_info("Hello, World");

	ASSERT_THAT(spy_string, HasSubstr("[INFO]"));
	ASSERT_THAT(spy_string, HasSubstr("Hello, World"));
}

TEST_F(LogKitTraceTest, setCustomTraceFunction)
{
	logger::set_trace_function(test_trace);

	log_info("Hello, World");

	ASSERT_THAT(spy_string, HasSubstr("Custom print function: "));
}
