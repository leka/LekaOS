// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::MockFunction;

using namespace leka;

class LogKitSinkTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	void TearDown() override { logger::set_sink_function(logger::internal::default_sink_function); }
};

TEST_F(LogKitSinkTest, useDefaultSinkFunction)
{
	log_info("Hello, World");
}

TEST_F(LogKitSinkTest, useCustomSinkFunction)
{
	MockFunction<logger::sink_function_t> mock_sink;

	EXPECT_CALL(mock_sink, Call(_, _)).Times(1);

	logger::set_sink_function(mock_sink.AsStdFunction());

	log_info("Hello, World");
}
