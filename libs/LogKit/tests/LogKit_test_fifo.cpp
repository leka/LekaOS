// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string>

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/mbed/FileHandle.h"
#include "stubs/mbed/Kernel.h"

using namespace leka;

using ::testing::HasSubstr;
using ::testing::MatchesRegex;

class LogKitFifoTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_sink_output = "";
		logger::set_sink_function(spy_sink_function);
		logger::set_filehandle_pointer(&mockfh);
	}

	void TearDown() override
	{
		logger::set_sink_function(logger::internal::default_sink_function);
		logger::set_filehandle_pointer(nullptr);
	}

	static void spy_sink_function(const char *str, std::size_t size)
	{
		spy_sink_output = std::string {str};
		std::cout << spy_sink_output;
	}

	static inline auto spy_sink_output = std::string {};
	mbed::mock::FileHandle mockfh	   = mbed::mock::FileHandle {};
};

TEST_F(LogKitFifoTest, processEmpty)
{
	EXPECT_CALL(mockfh, write).Times(0);

	logger::process_fifo();
}

TEST_F(LogKitFifoTest, processNotEmpty)
{
	EXPECT_CALL(mockfh, write).Times(128 / std::size(logger::buffer::process_buffer));

	for (auto i = 0; i < 128; ++i) {
		logger::buffer::fifo.push(static_cast<char>(i));
	}

	logger::process_fifo();
}

TEST_F(LogKitFifoTest, processFull)
{
	EXPECT_CALL(mockfh, write).Times(4096 / std::size(logger::buffer::process_buffer));

	for (auto i = 0; i < 4096; ++i) {
		logger::buffer::fifo.push(static_cast<char>(i));
	}

	logger::process_fifo();
}
