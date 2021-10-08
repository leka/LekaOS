// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::HasSubstr;
using ::testing::MockFunction;

using namespace leka;

class LogKitPrintTest : public ::testing::Test
{
  protected:
	void SetUp() override { spy_string = ""; }

	void TearDown() override { logger::set_print_function(logger::default_printf); }

	static void custom_printf(const char *str, std::size_t size) { has_custom_printf_been_called = true; }

	static void pull_data_from_fifo_buffer()
	{
		while (!logger::buffer::fifo.empty()) {
			char c {};
			logger::buffer::fifo.pop(c);
			spy_string.append({c});
		}
	}

	static inline auto spy_string = std::string {};

	static inline auto has_custom_printf_been_called = bool {false};
};

TEST_F(LogKitPrintTest, useDefaultPrintFunction)
{
	log_info("Hello, World");

	pull_data_from_fifo_buffer();

	ASSERT_THAT(spy_string, HasSubstr("[INFO]"));
	ASSERT_THAT(spy_string, HasSubstr("Hello, World"));
}

TEST_F(LogKitPrintTest, setCustomPrintFunction)
{
	logger::set_print_function(custom_printf);

	log_info("Hello, World");

	ASSERT_TRUE(has_custom_printf_been_called);
}
