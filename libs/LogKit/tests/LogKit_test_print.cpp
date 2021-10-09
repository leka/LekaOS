// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::MockFunction;

using namespace leka;

class LogKitTraceTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	void TearDown() override { logger::set_trace_function(logger::default_trace_function); }
};

TEST_F(LogKitTraceTest, useCustomTraceFunction)
{
	MockFunction<int64_t()> mock_trace;

	EXPECT_CALL(mock_trace, Call()).Times(1);

	logger::set_now_function(mock_trace.AsStdFunction());

	log_info("Hello, World");
}
