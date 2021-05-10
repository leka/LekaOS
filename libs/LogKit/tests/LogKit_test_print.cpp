// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::HasSubstr;

using namespace leka;

class LogKitPrintTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_string = "";
		logger::set_print_function(test_printf);
	}

	void TearDown() override { logger::set_print_function(logger::default_printf); }

	static void test_printf(const char *str, size_t size)
	{
		spy_string = "Custom print function: " + std::string {str};
		std::cout << spy_string;
	}

	static void test_default_printf(const char *str, size_t size)
	{
		spy_string = std::string {str};
		logger::default_printf(str, size);
	}

	static inline auto spy_string = std::string {};
};

TEST_F(LogKitPrintTest, useDefaultPrintFunction)
{
	logger::set_print_function(test_default_printf);

	log_info("Hello, World");

	ASSERT_THAT(spy_string, HasSubstr("[INFO]"));
	ASSERT_THAT(spy_string, HasSubstr("Hello, World"));
}

TEST_F(LogKitPrintTest, setCustomPrintFunction)
{
	logger::set_print_function(test_printf);

	log_info("Hello, World");

	ASSERT_THAT(spy_string, HasSubstr("Custom print function: "));
}
