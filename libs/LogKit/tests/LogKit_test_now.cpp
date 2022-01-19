// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string>

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/mbed/Kernel.h"

using namespace leka;

using ::testing::MockFunction;

class LogKitNowTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	void TearDown() override { logger::set_now_function(logger::internal::default_now_function); }
};

TEST_F(LogKitNowTest, useDefaultNowFunction)
{
	ASSERT_EQ(spy_kernel_tick_count, logger::internal::now());
}

TEST_F(LogKitNowTest, useCustomNowFunction)
{
	MockFunction<logger::now_function_t> mock_now;

	EXPECT_CALL(mock_now, Call()).Times(1);

	logger::set_now_function(mock_now.AsStdFunction());

	logger::internal::now();

	logger::set_now_function(logger::internal::default_now_function);
}
