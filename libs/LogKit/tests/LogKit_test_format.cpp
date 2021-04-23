// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string>

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/stub_Kernel.h"

using namespace leka;

using ::testing::ContainsRegex;
using ::testing::HasSubstr;
using ::testing::IsEmpty;
using ::testing::MatchesRegex;

class LogKitFormatTest : public ::testing::Test
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
		spy_string = std::string {str};
		std::cout << spy_string << '\n';
	}

	static inline auto spy_string = std::string {};
};

TEST_F(LogKitFormatTest, formatFullContentStringOnly)
{
	log_info("Hello, World");

	ASSERT_THAT(spy_string, MatchesRegex("[0-9:]+ \\[[A-Z]+\\] \\[.+:[0-9]+\\] .+() > .+"));
}

TEST_F(LogKitFormatTest, formatFullContentStringAdditionalArguments)
{
	log_info("Hello, World. %i %s!", 42, "FTW");

	ASSERT_THAT(spy_string, MatchesRegex("[0-9:]+ \\[[A-Z]+\\] \\[.+:[0-9]+\\] .+() > .+"));
}

TEST_F(LogKitFormatTest, formatFullContentStringEmpty)
{
	log_info("");

	ASSERT_THAT(spy_string, Not(HasSubstr(" > ")));
	ASSERT_THAT(spy_string, MatchesRegex("[0-9:]+ \\[[A-Z]+\\] \\[.+:[0-9]+\\] .+()"));
}

TEST_F(LogKitFormatTest, formatTimeHumanReadable)
{
	logger::format_time_human_readable(logger::now());

	auto time = std::string(leka::logger::buffer::timestamp.data());

	// Format: hhh:mm:ss:μμμ
	ASSERT_THAT(time, MatchesRegex("[0-9]{3}:[0-9]{2}:[0-9]{2}:[0-9]{3}"));
}

TEST_F(LogKitFormatTest, formatLevel)
{
	for (auto const [level, string]: leka::logger::level_lut) {
		// Format: [LEVEL]
		ASSERT_THAT(string, MatchesRegex("\\[[A-Z[:space:]]+\\]"));
		ASSERT_THAT(string, MatchesRegex("\\[(INFO|ERR |DBUG)\\]"));
	}
}

TEST_F(LogKitFormatTest, formatFileNameLineNumberFunctioName)
{
	logger::format_filename_line_function(__FILENAME__, __LINE__, __PRETTY_FUNCTION__);

	auto filename = std::string(leka::logger::buffer::filename.data());

	ASSERT_THAT(filename, MatchesRegex("\\[.+[.h|.cpp]:[0-9]+\\] .+"));
	ASSERT_THAT(filename, HasSubstr(__PRETTY_FUNCTION__));
}

TEST_F(LogKitFormatTest, formatMessageStringOnly)
{
	logger::format_message("Hello, World");

	auto message = std::string(leka::logger::buffer::message.data());

	ASSERT_THAT(message, MatchesRegex("> Hello, World"));
}

TEST_F(LogKitFormatTest, formatMessageStringAdditionalArguments)
{
	logger::format_message("%s, %s, with %i %s", "Hello", "World", 4, "variadic arguments");

	auto message = std::string(leka::logger::buffer::message.data());

	ASSERT_THAT(message, MatchesRegex("> Hello, World, with 4 variadic arguments"));
}

TEST_F(LogKitFormatTest, formatMessageStringEmpty)
{
	logger::format_message("");

	auto message = std::string(leka::logger::buffer::message.data());

	ASSERT_THAT(message, IsEmpty());
}

TEST_F(LogKitFormatTest, formatMessageNullPointer)
{
	logger::format_message(nullptr);

	auto message = std::string(leka::logger::buffer::message.data());

	ASSERT_THAT(message, IsEmpty());
}

TEST_F(LogKitFormatTest, formatMessageNoParameters)
{
	logger::format_message();

	auto message = std::string(leka::logger::buffer::message.data());

	ASSERT_THAT(message, IsEmpty());
}
