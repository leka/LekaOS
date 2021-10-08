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

class LogKitTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_string = "";
		logger::set_print_function(test_printf);
	}

	// void TearDown() override {}

	static void test_printf(const char *str, size_t size)
	{
		spy_string.append(str);
		std::cout << spy_string;
	}

	static void pull_data_from_fifo_buffer()
	{
		while (!logger::buffer::fifo.empty()) {
			char c {};
			logger::buffer::fifo.pop(c);
			spy_string.append({c});
		}
	}

	static inline auto spy_string = std::string {};
};

TEST_F(LogKitTest, logDebug)
{
	log_debug("Hello, World");

	pull_data_from_fifo_buffer();

	ASSERT_THAT(spy_string, HasSubstr("[DBUG]"));
	ASSERT_THAT(spy_string, HasSubstr("Hello, World"));
}

TEST_F(LogKitTest, logInfo)
{
	log_info("Hello, World");

	pull_data_from_fifo_buffer();

	ASSERT_THAT(spy_string, HasSubstr("[INFO]"));
	ASSERT_THAT(spy_string, HasSubstr("Hello, World"));
}

TEST_F(LogKitTest, logError)
{
	log_error("Hello, World");

	pull_data_from_fifo_buffer();

	ASSERT_THAT(spy_string, HasSubstr("[ERR ]"));
	ASSERT_THAT(spy_string, HasSubstr("Hello, World"));
}

TEST_F(LogKitTest, correctLineNumber)
{
	auto line = std::to_string(__LINE__ + 1);	// + 1 to count the next line
	log_info("Line number: %s", line.c_str());

	pull_data_from_fifo_buffer();

	ASSERT_THAT(spy_string, MatchesRegex(".+ \\[.+[.h|.cpp]:" + line + "\\] .*"));
}

TEST_F(LogKitTest, variadicArguments)
{
	log_info("%s, %s with %i %s", "Hello", "World", 4, "variadic arguments");

	pull_data_from_fifo_buffer();

	ASSERT_THAT(spy_string, MatchesRegex(".+ \\[INFO\\] \\[.*\\] .* > .* with 4 variadic arguments\n"));
}
