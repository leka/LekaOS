// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "commands/LedSingleCommand.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreLED.h"

using namespace leka;

class LedCommandTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreLED mockled {};

	LedSingleCommand ledcmd {mockled, mockled};
};

TEST(LedCommandTest, initialization)
{
	EXPECT_NE(&ledcmd, nullptr);
}
